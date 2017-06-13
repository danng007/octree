#include "fileWatch.h"


fileWatch::fileWatch()
{
//	char * path = "D:/LaserImaging/dataFiles/mfTest";
	
}


fileWatch::~fileWatch()
{
}
void fileWatch::RunFileWatch(string filePath, Manager * manager) {
	const char * path = filePath.c_str();
	LPTSTR folderPath = (LPTSTR)path;
	//WatchDirectory(folderPath);
	thisManager = manager;
	WatchByW(folderPath);
	
}
void fileWatch::WatchByW(LPTSTR lpDir)
{

	memset(notify, '\0', 1024);

	HANDLE handle_directory = CreateFile(
		lpDir,
		FILE_LIST_DIRECTORY,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS,
		NULL);
	if (handle_directory == INVALID_HANDLE_VALUE)
	{
		DWORD ERROR_DIR = GetLastError();
		MessageBox(NULL, TEXT("Error in openning directry!"), TEXT("File Watch"), 0);
	}
	BOOL watch_state;

	while (TRUE)
	{
		watch_state = ReadDirectoryChangesW(handle_directory,
			Notification,
			sizeof(notify),
			FALSE,
			FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE,
			&BytesReturned,
			NULL,
			NULL);



		if (GetLastError() == ERROR_INVALID_FUNCTION)
		{
			MessageBox(NULL, TEXT("System Error!"), TEXT("File Watch"), 0);
		}
		else if (watch_state == 0)
		{
			MessageBox(NULL, TEXT("Watching Error!"), TEXT("File Watch"), 0);
		}
		else if (GetLastError() == ERROR_NOTIFY_ENUM_DIR)
		{
			MessageBox(NULL, TEXT("Out of memorry!"), TEXT("File Watch"), 0);
		}
		else
		{

			//Save file name into a string
			string file_name;
			DWORD length = WideCharToMultiByte(0, 0, Notification->FileName, -1, NULL, 0, NULL, NULL);
			PSTR ps = new CHAR[length];
			if (length >= 0)
			{
				WideCharToMultiByte(0, 0, Notification->FileName, -1, ps, length, NULL, NULL);
				file_name = string(ps);
				delete[] ps;
			}

			//Reacion based on feedback action type

			if (Notification->Action == FILE_ACTION_ADDED)
			{
				cout << "Add file: " << file_name << endl;
				//manager->processFile(file_name);
				thisManager->pushPathToVector(file_name);
			}
			if (Notification->Action == FILE_ACTION_REMOVED)
			{
				cout << "Delete file: " << file_name << endl;
			}

			memset(Notification, '\0', 1024);

		}
		//Sleep(500);
	}

}
void fileWatch::WatchDirectory(LPTSTR lpDir)
{
	DWORD dwWaitStatus;
	HANDLE dwChangeHandles[2];
	TCHAR lpDrive[4];
	TCHAR lpFile[_MAX_FNAME];
	TCHAR lpExt[_MAX_EXT];

	_tsplitpath_s(lpDir, lpDrive, 4, NULL, 0, lpFile, _MAX_FNAME, lpExt, _MAX_EXT);

	lpDrive[2] = (TCHAR)'\\';
	lpDrive[3] = (TCHAR)'\0';

	// Watch the directory for file creation and deletion. 

	dwChangeHandles[0] = FindFirstChangeNotification(
		lpDir,                         // directory to watch 
		FALSE,                         // do not watch subtree 
		FILE_NOTIFY_CHANGE_ATTRIBUTES); // watch file name changes 

	if (dwChangeHandles[0] == INVALID_HANDLE_VALUE)
	{
		printf("\n ERROR: FindFirstChangeNotification function failed.\n");
		ExitProcess(GetLastError());
	}

	// Watch the subtree for directory creation and deletion. 

	dwChangeHandles[1] = FindFirstChangeNotification(
		lpDrive,                       // directory to watch 
		TRUE,                          // watch the subtree 
		FILE_NOTIFY_CHANGE_SIZE);  // watch dir name changes 

	if (dwChangeHandles[1] == INVALID_HANDLE_VALUE)
	{
		printf("\n ERROR: FindFirstChangeNotification function failed.\n");
		ExitProcess(GetLastError());
	}


	// Make a final validation check on our handles.

	if ((dwChangeHandles[0] == NULL) || (dwChangeHandles[1] == NULL))
	{
		printf("\n ERROR: Unexpected NULL from FindFirstChangeNotification.\n");
		ExitProcess(GetLastError());
	}

	// Change notification is set. Now wait on both notification 
	// handles and refresh accordingly. 

	while (TRUE)
	{
		// Wait for notification.

		printf("\nWaiting for notification...\n");

		dwWaitStatus = WaitForMultipleObjects(2, dwChangeHandles,
			FALSE, INFINITE);

		switch (dwWaitStatus)
		{
		case WAIT_OBJECT_0:

			// A file was created, renamed, or deleted in the directory.
			// Refresh this directory and restart the notification.

			RefreshDirectory(lpDir);
			if (FindNextChangeNotification(dwChangeHandles[0]) == FALSE)
			{
				printf("\n ERROR: FindNextChangeNotification function failed.\n");
				ExitProcess(GetLastError());
			}
			break;

		case WAIT_OBJECT_0 + 1:

			// A directory was created, renamed, or deleted.
			// Refresh the tree and restart the notification.

			RefreshTree(lpDrive);
			if (FindNextChangeNotification(dwChangeHandles[1]) == FALSE)
			{
				printf("\n ERROR: FindNextChangeNotification function failed.\n");
				ExitProcess(GetLastError());
			}
			break;

		case WAIT_TIMEOUT:

			// A timeout occurred, this would happen if some value other 
			// than INFINITE is used in the Wait call and no changes occur.
			// In a single-threaded environment you might not want an
			// INFINITE wait.

			printf("\nNo changes in the timeout period.\n");
			break;

		default:
			printf("\n ERROR: Unhandled dwWaitStatus.\n");
			ExitProcess(GetLastError());
			break;
		}
	}
}

void fileWatch::RefreshDirectory(LPTSTR lpDir)
{
	// This is where you might place code to refresh your
	// directory listing, but not the subtree because it
	// would not be necessary.

	_tprintf(TEXT("Directory (%s) changed.\n"), lpDir);
}

void fileWatch::RefreshTree(LPTSTR lpDrive)
{
	// This is where you might place code to refresh your
	// directory listing, including the subtree.

	_tprintf(TEXT("Directory tree (%s) changed.\n"), lpDrive);
}
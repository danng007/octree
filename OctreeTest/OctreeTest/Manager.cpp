#include "Manager.h"
#include<Windows.h>
#include <Psapi.h>
#pragma comment (lib,"Psapi.lib")
#include <tchar.h>
#include <string.h>
#include <psapi.h>
#include <strsafe.h>
#define BUFSIZE 512
Manager::Manager()
{
}


Manager::~Manager()
{
}
BOOL GetFileNameFromHandle(HANDLE hFile)
{
	BOOL bSuccess = FALSE;
	TCHAR pszFilename[MAX_PATH + 1];
	HANDLE hFileMap;

	// Get the file size.
	DWORD dwFileSizeHi = 0;
	DWORD dwFileSizeLo = GetFileSize(hFile, &dwFileSizeHi);

	if (dwFileSizeLo == 0 && dwFileSizeHi == 0)
	{
		_tprintf(TEXT("Cannot map a file with a length of zero.\n"));
		return FALSE;
	}

	// Create a file mapping object.
	hFileMap = CreateFileMapping(hFile,
		NULL,
		PAGE_READONLY,
		0,
		1,
		NULL);

	if (hFileMap)
	{
		// Create a file mapping to get the file name.
		void* pMem = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 1);

		if (pMem)
		{
			if (GetMappedFileName(GetCurrentProcess(),
				pMem,
				pszFilename,
				MAX_PATH))
			{

				// Translate path with device name to drive letters.
				TCHAR szTemp[BUFSIZE];
				szTemp[0] = '\0';

				if (GetLogicalDriveStrings(BUFSIZE - 1, szTemp))
				{
					TCHAR szName[MAX_PATH];
					TCHAR szDrive[3] = TEXT(" :");
					BOOL bFound = FALSE;
					TCHAR* p = szTemp;

					do
					{
						// Copy the drive letter to the template string
						*szDrive = *p;

						// Look up each device name
						if (QueryDosDevice(szDrive, szName, MAX_PATH))
						{
							size_t uNameLen = _tcslen(szName);

							if (uNameLen < MAX_PATH)
							{
								bFound = _tcsnicmp(pszFilename, szName, uNameLen) == 0
									&& *(pszFilename + uNameLen) == _T('\\');

								if (bFound)
								{
									// Reconstruct pszFilename using szTempFile
									// Replace device path with DOS path
									TCHAR szTempFile[MAX_PATH];
									StringCchPrintf(szTempFile,
										MAX_PATH,
										TEXT("%s%s"),
										szDrive,
										pszFilename + uNameLen);
									StringCchCopyN(pszFilename, MAX_PATH + 1, szTempFile, _tcslen(szTempFile));
								}
							}
						}

						// Go to the next NULL character.
						while (*p++);
					} while (!bFound && *p); // end of string
				}
			}
			bSuccess = TRUE;
			UnmapViewOfFile(pMem);
		}

		CloseHandle(hFileMap);
	}
	_tprintf(TEXT("File name is %s\n"), pszFilename);
	return(bSuccess);
}
void Manager::mainRun() {
	while (true)
	{
		if (filePaths.empty())
		{
//			Sleep(100); //current no file loading, sleep for waiting
			continue;
		}
		else {
			string currentPath;
			cout << "Size: "<<filePaths.size()<<endl;
			currentPath = filePaths.back();
			filePaths.pop_back();
			processFile(currentPath);
			countHandle();
		}
	}
}

void Manager::countHandle() {
	DWORD type_char = 0,
		type_disk = 0,
		type_pipe = 0,
		type_remote = 0,
		type_unknown = 0,
		handles_count = 0;
	BY_HANDLE_FILE_INFORMATION lpFileInformation;
	char lpFileName[1000];
	DWORD nSize=0;
	
	GetProcessHandleCount(GetCurrentProcess(), &handles_count);
	handles_count *= 4;
	for (DWORD handle = 0x4; handle < handles_count; handle += 4) {
		switch (GetFileType((HANDLE)handle)) {
		case FILE_TYPE_CHAR:
			type_char++;
			break;
		case FILE_TYPE_DISK:
			//CloseHandle((HANDLE)handle);
			type_disk++;
			cout << handle<<endl;
			//nSize=GetFileSize((HANDLE)handle, NULL);
			//cout << "File Info: " << nSize << endl;
			//GetFileNameFromHandle((HANDLE)handle);
			//创建Mapping对象  
			//GetCurrentDirectory(nSize, lpFileName);
			//GetModuleFileName( NULL, lpFileName, nSize);
			//GetFileInformationByHandle((HANDLE)handle, &lpFileInformation);
			/*if (GetModuleFileNameEx((HANDLE)handle, NULL, lpFileName, nSize))
			{
				cout << "File Info: "<< lpFileName << endl;
			}*/
			
			break;
		case FILE_TYPE_PIPE:
			type_pipe++;
			break;
		case FILE_TYPE_REMOTE:
			type_remote++;
			break;
		case FILE_TYPE_UNKNOWN:
			if (GetLastError() == NO_ERROR) type_unknown++;
			break;

		}
	}
	cout << "Handle: "<<type_char << " " << type_disk << " " << type_pipe << " " << type_remote << endl;
}
void Manager::initialIndex(vector<long long int> &index, vector<int> &levels, vector<long long int> &totalIndex, int maxdepth) {

	totalIndex[maxdepth - 1] = 1;
	index[maxdepth - 1] = 0;
	levels[maxdepth - 1] = 8;
	for (int i = 1; i < maxdepth; i++)
	{
		index[i] = 0;
		levels[i] = 8;
		totalIndex[maxdepth - 1 - i] = pow(8, i) + totalIndex[maxdepth - i];
	}
}
void Manager::processFile(string filePath) {
	path1 = basePath + filePath;
	cout << "\nProcessing: " << path1 << endl;
	timeBegin = clock();
	vector<long long>index(maxdepth, 1);
	vector<long long>totalIndex(maxdepth, 1);
	vector<int>levels(maxdepth, 1);
	vector<long long>index2(maxdepth, 1);
	vector<long long>totalIndex2(maxdepth, 1);
	vector<int>levels2(maxdepth, 1);
	multiFiles *mf = new multiFiles();
	mf->initialBackRootFiles();
 	mf->splitToFolders(path1);
	mf->deleteEmptyFiles();
	initialIndex(index, levels, totalIndex, maxdepth);
	runThread(levels, index, totalIndex, maxdepth);
	mf->deleteAllFiles();
	timeLast = (clock() - timeBegin)*1.0 / CLOCKS_PER_SEC;
	cout << "\n*************************************" << endl;
	cout << "File Processing Time: " << timeLast << " s" << endl;
	cout << "*************************************" << endl;
}
void Manager::pushPathToVector(string addPath) {
	filePaths.push_back(addPath);
}
void Manager::runThread(vector<int> levels, vector<long long int> index, vector<long long int> totalIndex, int maxdepth) {
	char resultV = 0;
	string files;
	string childFile = "";
	int fileLength;
	string subFile;
	DIR *dir;
	struct dirent *ent;
	int indexV;
	subFile = "D:/LaserImaging/dataFiles/mfTest/seperateFiles/";
	dir = opendir(subFile.c_str());
	if (dir != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			switch (ent->d_type) {
			case DT_REG:
				files = ent->d_name;
				fileLength = files.length();
				if (fileLength > 6)
				{
					threadRun(files, levels, index, totalIndex, maxdepth);
				}
				break;
			case DT_DIR:
				break;
			default:
				printf("%s:\n", ent->d_name);
			}
		}
	}
	closedir(dir);
}

void Manager::threadRun(string dataFilePath, vector<int> levels, vector<long long int> index, vector<long long int> totalIndex, int maxdepth) {
	double xmin = -100, ymin = -100, zmin = -100, xmax = 100, ymax = 100, zmax = 100;
	double timeLast;
	double timeBegin;
	string line;
	string filePathForThread;
	float x, y, z, value, a;
	int r, g, b;
	ifstream dataFile;
	filePathForThread = "D:/LaserImaging/dataFiles/mfTest/seperateFiles/" + dataFilePath;
	dataFile.open(filePathForThread);
	templateBin *tb = new templateBin(xmin, xmax, ymin, ymax, zmin, zmax, 0.01, maxdepth);
	tb->setBase(dataFilePath);
	tb->initialStreams();
	timeBegin = clock();
	int counter = 0;

	while (getline(dataFile, line))
	{
		counter++;
		//cout << "Inside" << endl;
		istringstream iss(line);

		if (!(iss >> x >> y >> z >> r >> g >> b >> a)) { break; } // error
		a = a * 100;
		tb->calIndex(x, y, z, r, g, b, a, levels, index, totalIndex);
	}
	
	tb->closeStreams();
	tb->createAllHrc();
	dataFile.close();
	//tb->~templateBin();

	timeLast = (clock() - timeBegin)*1.0 / CLOCKS_PER_SEC;
	//cout << "*************************************" << endl;
	//cout << "Single Thread Finish Processing Time: " << timeLast << " s" << endl;
	cout << "Processing Points: " << counter << endl;
	//cout << "*************************************" << endl;
}
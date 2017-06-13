#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include<iostream>
#include <tchar.h>
#include <string>
#include<time.h>
#include "Manager.h"
using namespace std;

class fileWatch
{
public:
	fileWatch();
	~fileWatch();
	void RunFileWatch(string filePath, Manager* manager);
private:
	void RefreshDirectory(LPTSTR);
	void RefreshTree(LPTSTR);
	void WatchDirectory(LPTSTR);
	void WatchByW(LPTSTR);
	//Parameters define for ReadDirectoryChangesW
	char notify[1024];
	Manager * thisManager;
	FILE_NOTIFY_INFORMATION *Notification = (FILE_NOTIFY_INFORMATION *)notify;
	FILE_NOTIFY_INFORMATION *TempNotification = NULL;
	DWORD BytesReturned = 0;
	DWORD version = 0;
};

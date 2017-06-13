#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <thread>
#include "fileWatch.h"
#include "Manager.h"
using namespace std;
//Define Octree Class
void runFileWatch(string path, Manager * manager) {
	fileWatch *fw = new fileWatch();
	fw->RunFileWatch(path, manager);
}
void runManager(Manager * manager) {
	manager->mainRun();
}
int main()
{
	_setmaxstdio(1000);
	Manager * manager = new Manager();
	string path = "D:/LaserImaging/dataFiles/mfTest";
	thread t1(runFileWatch, path, manager);
	thread t2(runManager, manager);
	t1.join();
	cout << "Thread one done." << endl;
	t2.join();
	cout << "Thread two done." << endl;
	system("pause");
}
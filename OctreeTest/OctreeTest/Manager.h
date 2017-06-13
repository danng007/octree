#ifndef MANAGER_H
#define MANAGER_H
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include<math.h>
#include<vector>
#include<fstream>
#include<sstream>
#include<time.h>
#include<direct.h>
#include"multiFiles.h"
#include "templateBin.h"
using namespace std;
class Manager
{
public:
	Manager();
	~Manager();
	void processFile(string filePath);
	void pushPathToVector(string addPath);
	void mainRun();
private:
	void countHandle();
	void initialIndex(vector<long long int> &index, vector<int> &levels, vector<long long int> &totalIndex, int maxdepth);
	void threadRun(string dataFilePath, vector<int> levels, vector<long long int> index, vector<long long int> totalIndex, int maxdepth);
	void runThread(vector<int> levels, vector<long long int> index, vector<long long int> totalIndex, int maxdepth);
	string basePath = "D:\\LaserImaging/dataFiles/mfTest/";
	string path1;
	vector<string> filePaths;
	//string path2 = "D:/LaserImaging/dataFiles/thretest2.txt";
	double timeLast;
	double timeBegin;
	int maxdepth = 8;
	int indexCounter = 0;
	
};
#endif
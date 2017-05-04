#pragma once
#include <iostream>
#include <stdio.h>
#include<math.h>
#include<vector>
#include<fstream>
#include<sstream>
#include<iterator>
#include<algorithm>
#include<time.h>
#include<direct.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "dirent.h"
#include<time.h>
#include<utility>
#include<tuple>
#include <sstream>
using namespace std;


class templateBin
{
public:
	int folderSplitLevel;
	templateBin(double Xmin, double Xmax, double Ymin, double Ymax, double Zmin, double Zmax, float Scale, int Maxdepth);
	~templateBin();
	double xmin = 0, xmax = 0, ymin = 0, ymax = 0, zmin = 0, zmax = 0;
	void calIndex(float x, float y, float z, unsigned char r, unsigned char g, unsigned char b, unsigned char a, vector<int> levels, vector<long long int> index, vector<long long int> totalIndex);
	string basePath = "D:/LaserImaging/dataFiles/";
	string folderBase = "D:/LaserImaging/dataFiles/mfResult";
	float scale = 1;
	void createAllHrc();
	ofstream *binWriter;
	ofstream *rootWriter;
	ifstream *hrcReader;
	void createHrc(string hrcName, string folderStr, int startLevel);
	void initialStreams();
	void closeStreams();
	void setBase(string newFileBasePath);
private:
	int newPosition[3] = { 0 };
	bool storeStream;
	stringstream sstm;
	ofstream* rootfile;
	ofstream* levelOneStreams[8];
	ofstream* levelTwoStreams[8][8];
	char buffer[5];
	int calLevelIndex(int depth, int length, int width);
	typedef int SPACE2D_t[129][129];
	SPACE2D_t* arr = new SPACE2D_t[129];
	ifstream removeFileCheck;
	string removeFilePath;
	//template<int> Array3D arr(256);
	bool is_empty(std::ifstream& pFile);
	int maxDepth;
	char returnChildHrc(string& fileFolderPath, string& filePath, int level);
	char returnFinChildHrc(string& filePath, int level);
	float x, y, z;
	void writeBinValue(float x, float y, float z, int r, int g, int b, int a, string& filePath, float newMinX, float newMinY, float newMinZ, int levelNow, vector<int> levels);
	int countRows(string& filePath);
	
};


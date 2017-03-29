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
using namespace std;
class templateBin
{
public:
	templateBin(double Xmin, double Xmax, double Ymin, double Ymax, double Zmin, double Zmax, float Scale, int Maxdepth);
	~templateBin();
	double xmin = 0, xmax = 0, ymin = 0, ymax = 0, zmin = 0, zmax = 0;
	void calIndex(float x, float y, float z, unsigned char r, unsigned char g, unsigned char b, unsigned char a, vector<int> levels, vector<long long int> index, vector<long long int> totalIndex);
	string basePath = "./dataFiles/r/";
	float scale = 1;
	void createAllHrc();
	ofstream *binWriter;
	void createHrc(string hrcName, string folderStr, int startLevel);
	
private:
	
	int calLevelIndex(int depth, int length, int width);
	//int arg[4][40] = { { 8,7,8,6,8,7,8,5,8,7,8,6,8,7,8,4,8,7,8,6,8,7,8,5,8,7,8,6,8,7,8,4,8,7,8,6,8,7,8,5 },{ 7,8,5,8,7,8,6,8,7,8,4,8,7,8,6,8,7,8,5,8,7,8,6,8,7,8,4,8,7,8,6,8,7,8,5,8,7,8,6,8 },{ 8,7,8,6,8,7,8,4,8,7,8,6,8,7,8,5,8,7,8,6,8,7,8,4,8,7,8,6,8,7,8,5,8,7,8,6,8,7,8,4 },{ 7,8,4,8,7,8,6,8,7,8,5,8,7,8,6,8,7,8,4,8,7,8,6,8,7,8,5,8,7,8,6,8,7,8,4,8,7,8,6,8 } };
	int arg[40][40][40] = { 0 };
	int maxDepth;
	char returnChildHrc(string& filePath, int level);
	char returnFinChildHrc(string& filePath, int level);
	float x, y, z;
	void writeBinValue(float x, float y, float z, int r, int g, int b, int a, string& filePath, float newMinX, float newMinY, float newMinZ);
	int countRows(string& filePath);
	
};


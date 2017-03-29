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
using namespace std;
class writeBin {
public:
	writeBin(double Xmin, double Xmax, double Ymin, double Ymax, double Zmin, double Zmax, float scale);
	void calIndex(float x, float y, float z, unsigned char r, unsigned char g, unsigned char b, unsigned char a, vector<int> levels, vector<long long int> index, vector<long long int> totalIndex, int maxdepth);
	//void iniMinMax(double Xmin, double Xmax, double Ymin, double Ymax, double Zmin, double Zmax);
	double xmin = 0, xmax = 0, ymin = 0, ymax = 0, zmin = 0, zmax = 0;
	float scale = 1;
	void loopAllDir();
	string basePath = "./dataFiles/r/";
private:
	void writeBinValue(float x, float y, float z, int r, int g, int b, int a, string& filePath);
	void createParentFile(string& folderName, string& filePath);
	
	float x, y, z;
	
};
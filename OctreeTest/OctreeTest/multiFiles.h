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

class multiFiles
{
public:
	multiFiles();
	~multiFiles();
	void splitToFolders(string filePath);
	void initialBackRootFiles();
	void deleteEmptyFiles();
private:
	stringstream sstm;
	string folderBase = "D:/LaserImaging/dataFiles/test/";
	ofstream* backRootStreams[7][7][7];
	
	bool multiFiles::is_empty(std::ifstream& pFile);
};


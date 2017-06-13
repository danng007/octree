#pragma once
#include <iostream>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <string>
#include<sstream>
#include <stdlib.h>
#include<iterator>
using namespace std;
class fileIO
{
public:
	fileIO();
	~fileIO();
	int fileIn(string fileName);
	vector<vector<float>> getFileValues();
	int fileOut(vector<vector<float>> fileValuesIn, string fileName);
private:
	vector<vector<float>> fileValues;

};


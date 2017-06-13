#pragma once
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <fstream>
#include <string>
#include<sstream>
#include <stdlib.h>
#include<iterator>
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#define MAX_SOURCE_SIZE (0x100000)
using namespace std;
class GPUProcess
{
public:
	GPUProcess();
	~GPUProcess();
	void run();
	void getFileValues(vector<vector<float>> valuesIn, int sizeIn, int rowSizeIn);
private:
	void resizeFileValue();
	float *fileValue1D, *result1D;
	vector<vector<float>> fileValues;
	int fileRows, rowSize, fileSize, resultSize, outputRowSize;
	void outputResultToFile();
	int convertToString(const char *filename, std::string& s);
};


#include "templateBin.h"



templateBin::templateBin(double Xmin, double Xmax, double Ymin, double Ymax, double Zmin, double Zmax, float Scale, int Maxdepth)
{
	xmax = Xmax;
	xmin = Xmin;
	ymin = Ymin;
	ymax = Ymax;
	zmin = Zmin;
	zmax = Zmax;
	scale = Scale;
	maxDepth = Maxdepth;

	xmin /= scale;
	xmax /= scale;
	ymin /= scale;
	zmin /= scale;
	ymax /= scale;
	zmax /= scale;
	for (int i = 0; i < 128; i++)
	{
		for (int j = 0; j < 128; j++) {
			for (int m = 0; m < 128; m++)
			{
				arr[i][j][m] = 0;
			}
		}
	}
}

templateBin::~templateBin()
{
	delete[] arr;
}

void templateBin::createAllHrc() {
	delete[] arr;
	int skipCounter = 2;
	string rootHrcName = "r.hrc";
	createHrc(rootHrcName, basePath, 0);
	string hrcName = "";
	string folderStr = "";
	//	createHrc(hrcName,folderStr,7);
	string files;
	DIR *dir;
	struct dirent *ent;
	dir = opendir(basePath.c_str());
	if (dir != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			switch (ent->d_type) {
			case DT_REG:
				break;
			case DT_DIR:
				if (skipCounter > 0)
				{
					skipCounter--;
					break;
				}
				hrcName = ent->d_name;
				folderStr = basePath + hrcName + "/";
				hrcName = "r" + hrcName + ".hrc";
				createHrc(hrcName, folderStr, maxDepth - 1);
				break;
			default:
				printf("%s:\n", ent->d_name);
			}
		}
	}
	closedir(dir);

}
void templateBin::createHrc(string hrcName, string folderStr, int startLevel) {

	//ofstream *hrcWriter;
	//string hrcName = "r.hrc";

	binWriter = new ofstream(folderStr + hrcName, ios::out | ios::binary | ofstream::app);
	//*binWriter << fflush;

	char childV = 0;
	string countFile = "";
	string files;
	DIR *dir;
	struct dirent *ent;
	int fileLength = 0;
	int rowCounter;
	for (int i = 0; i < maxDepth; i++)
	{
		dir = opendir(folderStr.c_str());
		if (dir != NULL) {
			while ((ent = readdir(dir)) != NULL) {
				switch (ent->d_type) {
				case DT_REG:

					files = ent->d_name;
					if (files.compare(hrcName) == 0)
					{
						break;
					}
					fileLength = files.length();
					if (fileLength == i + 5 + startLevel)
					{
						//cout << files << " " << fileLength <<endl;//char 1 = int 49;
						childV = returnChildHrc(files, i);
						binWriter->write((const char*)(&childV), 1);
						rowCounter = countRows(folderStr + files);
						binWriter->write((const char*)(&rowCounter), 4);
						binWriter->flush();
						//cout << files << " : " <<childV<< endl;
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
		childV = 0;
	}


	dir = opendir(folderStr.c_str());
	if (dir != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			switch (ent->d_type) {
			case DT_REG:
				break;
			case DT_DIR:
				files = ent->d_name;
				fileLength = files.length();
				if (fileLength == 5)
				{
					//cout << files << " " << fileLength << endl;//char 1 = int 49;
					childV = returnFinChildHrc(files, maxDepth - 1);
					binWriter->write((const char*)(&childV), 1);
					countFile = folderStr + files + "/r" + files + ".bin";
					rowCounter = countRows(countFile);
					binWriter->write((const char*)(&rowCounter), 4);
					binWriter->flush();
					//cout << files << " : " << childV << endl;
				}
				break;
			default:
				printf("%s:\n", ent->d_name);
			}
		}
	}

	closedir(dir);
	childV = 0;
	binWriter->close();
}
char templateBin::returnChildHrc(string& filePath, int level) {
	char resultV = 0;
	string files;
	string childFile = "";
	int fileLength;
	DIR *dir;
	struct dirent *ent;
	int indexV;
	dir = opendir(basePath.c_str());
	if (dir != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			switch (ent->d_type) {
			case DT_REG:
				files = ent->d_name;
				fileLength = files.length();
				if (fileLength == level + 1 + 5)
				{
					childFile = filePath.substr(0, level + 1) + files[level + 1] + filePath.substr(level + 1, level + 4);
					if (childFile.compare(files) == 0)
					{
						indexV = int(files[level + 1]) - 48;
						//cout << files << " " << fileLength << " " << indexV << endl;//char 1 = int 49;

						resultV = resultV | (1 << indexV);
					}
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
	return resultV;
}
char templateBin::returnFinChildHrc(string& filePath, int level) {
	char resultV = 0;
	string files;
	string childFile = "";
	int fileLength;
	string subFile;
	DIR *dir;
	struct dirent *ent;
	int indexV;
	subFile = basePath + filePath + "/";
	dir = opendir(subFile.c_str());
	if (dir != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			switch (ent->d_type) {
			case DT_REG:
				files = ent->d_name;
				fileLength = files.length();
				if (fileLength == level + 1 + 5)
				{
					childFile = filePath.substr(0, level + 1) + files[level + 1] + filePath.substr(level + 1, level + 4);
					if (childFile.compare(files) == 0)
					{
						indexV = int(files[level + 1]) - 48;
						//cout << files << " " << fileLength << " " << indexV << endl;//char 1 = int 49;

						resultV = resultV | (1 << indexV);
					}
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
	return resultV;
}

int templateBin::countRows(string& filePath) {
	int result = 0;
	ifstream *binReader;
	char *buffer = new char[16];
	binReader = new ifstream(filePath, ios::in | ios::binary);
	//binReader->unsetf(std::ios_base::skipws);
	// count the newlines with an algorithm specialized for counting:
	while (binReader->good()) {
		binReader->read(buffer, 16); //1 size byte for 2 number
		result++;
	}
	return result - 1;
}

int templateBin::calLevelIndex(int depth, int length, int width) {
	depth = (xmax - depth) / (xmax - xmin) * 128;
	length = (ymax - length) / (ymax - ymin) * 128;
	width = (zmax - width) / (zmax - zmin) * 128;
	if (depth >= 128 || length >= 128 || width>=128)
	{
		int testt;
		testt = 1;

	}
	
	arr[depth][length][width]++;
	//cout << depth << " " << length << " " << width <<" "<< arr[depth][length][width]<< endl;
	if (arr[depth][length][width] < maxDepth)
	{
		return arr[depth][length][width];
	}
	else
	{
		return maxDepth;
	}
	//return arr[depth][length][width];
	//return 1;

}
void templateBin::calIndex(float xin, float yin, float zin, unsigned char r, unsigned char g, unsigned char b, unsigned char a, vector<int> levels, vector<long long int> index, vector<long long int> totalIndex)
{

	/*
	*   3----7
	*  /|   /|
	* 2----6 |
	* | 1--|-5
	* |/   |/
	* 0----4
	*/
	if (yin<0)
	{
		yin = -yin;
	}
	string filePath = "";
	x = xin / scale;
	y = yin / scale;
	z = zin / scale;

	int levelNow = calLevelIndex((int)x, (int)y, (int)z);

	float midX = (xmax - xmin) / 2 + xmin;
	float midY = (ymax - ymin) / 2 + ymin;
	float midZ = (zmax - zmin) / 2 + zmin;

	string indexs = "";
	string folderIndex = "";
	string dest = basePath;
	stringstream ss;
	float newMinX = xmin, newMinY = ymin, newMinZ = zmin, halfEdge = (xmax - xmin), newMaxX = xmax, newMaxY = ymax, newMaxZ = zmax;

	for (int i = 1; i < levelNow; i++) {

		if (x <= midX)
		{
			levels[i] = levels[i] - 4;
			newMaxX = midX;
			midX = (midX - newMinX) / 2 + newMinX;

		}
		else
		{
			newMinX = midX;
			midX = midX + (newMaxX - midX) / 2;
		}

		if (y <= midY)
		{
			levels[i] = levels[i] - 2;
			newMaxY = midY;
			midY = (midY - newMinY) / 2 + newMinY;

		}
		else
		{
			newMinY = midY;
			midY = midY + (newMaxY - midY) / 2;
		}

		if (z <= midZ)
		{
			levels[i] = levels[i] - 1;
			newMaxZ = midZ;
			midZ = (midZ - newMinZ) / 2 + newMinZ;

		}
		else
		{
			newMinZ = midZ;
			midZ = midZ + (newMaxZ - midZ) / 2;
		}

		index[i] = index[i - 1] + (levels[i] - 1)*totalIndex[i] + 1;
		//cout << "Levels" << i << " In: " << levels[i] << " Index: " << index[i] << endl;
		indexs += to_string(levels[i] - 1);

	}
	if (!indexs.compare("16626"))
	{
		int testt;
		testt = 0;
	}
	//if (levelNow >= 7)
	//{
	//	/*if (levels[4]==3&&levels[5]==7)
	//	{
	//		int tests = 0;
	//	}
	//	for (int j = 1; j < 6; j++)
	//	{
	//		folderIndex += to_string(levels[j] - 1);
	//	}*/
	//	dest += indexs;
	//	_mkdir(dest.c_str());
	//	filePath = dest + "/r" + indexs + ".bin";
	//}
	//else
	//{
	filePath = basePath + "r" + indexs + ".bin";
	//}
	//cout << levelNow << " " << newMinX << " " << newMinY << " " << newMinZ << endl;;

	writeBinValue(x, y, z, r, g, b, a, filePath, newMinX, newMinY, newMinZ);

}
void templateBin::writeBinValue(float x, float y, float z, int r, int g, int b, int a, string& filePath, float newMinX, float newMinY, float newMinZ) {
	x -= newMinX;
	y -= newMinY;
	z -= newMinZ;
	binWriter = new ofstream(filePath, ios::out | ios::binary | ofstream::app);
	if (y*scale + ymin > ymax)
	{
		int a = 1;
	}
	//cout << x << " " << y << " " << z << endl;
	int pos[3] = { x,y,z };
	binWriter->write((const char*)pos, 3 * sizeof(int));
	unsigned char rgba[4] = { r,g,b,a };
	binWriter->write((const char*)rgba, 4 * sizeof(unsigned  char));
	binWriter->flush();
	binWriter->close();
}
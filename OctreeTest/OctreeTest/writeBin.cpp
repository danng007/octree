#include "writeBin.h"



writeBin::writeBin(double Xmin, double Xmax, double Ymin, double Ymax, double Zmin, double Zmax, float Scale) {
	xmax = Xmax;
	xmin = Xmin;
	ymin = Ymin;
	ymax = Ymax;
	zmin = Zmin;
	zmax = Zmax;
	scale = Scale;
}
void writeBin::loopAllDir() {
	int skipCounter = 2;
	string folderName;
	string files;
	DIR *dir;
	struct dirent *ent;
	dir = opendir(basePath.c_str());
	if (dir != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL) {
			switch (ent->d_type) {
			case DT_REG:
				//printf("%*.*s\n", ent->d_namlen, ent->d_namlen, ent->d_name);
				break;
			case DT_DIR:
				if (skipCounter > 0)
				{
					skipCounter--;
					continue;
				}
				files = string(ent->d_name);
				folderName = basePath+files;
				files.pop_back();
				files = basePath +"r"+ files + ".bin";
				cout << "Parent Loop: " << files << endl;
				createParentFile( folderName, files);
				
				break;

			default:
				printf("%s:\n", ent->d_name);
			}
		}
		closedir(dir);
	}
}
//ent->d_name only return file name, not whole path;
void writeBin::createParentFile(string& folderName, string& filePath) {
	int testCounter = 0;
	string line;
	DIR *dir;
	struct dirent *ent;
	string files;
	char* buffer;
	ofstream *binWriter;
	binWriter = new ofstream(filePath, ios::out | ios::binary| ofstream::app);
	ifstream *binReader;
	bool skipC = false;
	dir = opendir(folderName.c_str());
	if (dir != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL) {
			switch (ent->d_type) {
			case DT_REG:
				cout << "Child Loop 1: "<<ent->d_name << endl;
				files = folderName + "/" + string(ent->d_name);
				binReader = new ifstream(files, ios::in|ios::binary);
				//binReader->unsetf(std::ios_base::skipws);
				// count the newlines with an algorithm specialized for counting:
				while (binReader->good()) {
					if (skipC)
					{
						
						buffer = new char[16];
						binReader->read(buffer, 16); //1 size byte for 2 number
						binWriter->write(buffer, 16);
						skipC = false;
					}
					else
					{
						buffer = new char[16];
						binReader->read(buffer, 16);
						skipC = true;
					}
					binWriter->flush();
				}
				break;
			case DT_DIR:
				//cout << "Child Loop 2: " << ent->d_name << endl;
				break;

			default:
				cout << "Child Loop 3: " << ent->d_name << endl;
			}
		}
		
		cout << "Test Counter " << testCounter << endl;
		closedir(dir);
	}
}
void writeBin::calIndex(float xin, float yin, float zin, unsigned char r, unsigned char g, unsigned char b, unsigned char a, vector<int> levels, vector<long long int> index, vector<long long int> totalIndex, int maxdepth)
{
	x = xin * 100;
	y = yin * 100;
	z = zin * 100;

	float midX = (xmax - xmin) / 2 + xmin;
	float midY = (ymax - ymin) / 2 + ymin;
	float midZ = (zmax - zmin) / 2 + zmin;

	string indexs = "";
	string dest = basePath;
	stringstream ss;
	for (int i = 1; i < maxdepth; i++) {
		if (x <= midX)
		{
			levels[i] = levels[i] - 4;
			midX = (midX - xmin) / 2 + xmin;
		}
		else
		{
			midX = midX + (xmax - midX) / 2;
		}
		if (y <= midY)
		{
			levels[i] = levels[i] - 2;
			midY = (midY - ymin) / 2 + ymin;
		}
		else
		{
			midY = midY + (ymax - midY) / 2;
		}
		if (z <= midZ)
		{
			levels[i] = levels[i] - 1;
			midZ = (midZ - zmin) / 2 + zmin;
		}
		else
		{
			midZ = midZ + (zmax - midZ) / 2;
		}
		index[i] = index[i - 1] + (levels[i] - 1)*totalIndex[i] + 1;
		//cout << "Levels" << i << " In: " << levels[i] << " Index: " << index[i] << endl;
		indexs += to_string(levels[i]);
		//uncommand this one, full data will write into file

	}
	dest += indexs;
	//cout << dest << endl;
	_mkdir(dest.c_str());
	string filePath = dest + "/r" + indexs + ".bin";
	writeBinValue(x,y,z,r,g,b,a,filePath);

}

void writeBin::writeBinValue(float x, float y, float z, int r, int g, int b, int a, string& filePath) {
	ofstream *binWriter;
	binWriter = new ofstream(filePath, ios::out | ios::binary | ofstream::app);
	//cout << x << " " << y << " " << z << endl;
	int pos[3] = { x,y,z };
	binWriter->write((const char*)pos, 3 * sizeof(int));
	unsigned char rgba[4] = { r,g,b,a };
	binWriter->write((const char*)rgba, 4 * sizeof(unsigned  char));
	binWriter->flush();
	//calIndex(x, y, z, levels, index, totalIndex, maxdepth, outputFile);
		//Comment: process pair(a, b)
}

#include "multiFiles.h"



multiFiles::multiFiles()
{
}


multiFiles::~multiFiles()
{
}
void multiFiles::initialBackRootFiles() {
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++) {
			for (int l = 0; l < 7; l++)
			{
				sstm.str("");
				sstm << folderBase  <<"_"<< i-3 <<"_"<< j-3 << "_"<< l-3 <<".txt";
				backRootStreams[i][j][l] = new ofstream(sstm.str(), ios::out|ofstream::app);
			}
		}
	}
}
void multiFiles::splitToFolders(string filePath) {
	string line;
	ifstream dataFile;
	dataFile.open(filePath);
	float x, y, z, a;
	int r, g, b;
	int counter = 0;
	int basePosition[3] = { 0 };
	while (getline(dataFile, line))
	{
		counter++;
		istringstream iss(line);
		if (!(iss >> x >> y >> z >> r >> g >> b >> a)) { break; } // error

		basePosition[0] = x / 100 ;
		basePosition[1] = y / 100 ;
		basePosition[2] = z / 100 ;
		if (basePosition[0] <-3 || basePosition[0] > 3 || basePosition[1] <-3 || basePosition[1] > 3 || basePosition[2] <-3 || basePosition[2] > 3 )
		{
			sstm.str("");
			sstm << folderBase << "_" << basePosition[0]<< "_" << basePosition[1] << "_" << basePosition[2]<< ".txt";
			ofstream currentWriteFile(sstm.str());
			currentWriteFile << line+"\n";
			currentWriteFile.close();
		}
		else {
			*backRootStreams[basePosition[0] + 3][basePosition[1] + 3][basePosition[2] + 3] << line+"\n";
		}
		
	}
}
void multiFiles::deleteEmptyFiles() {
	string removeFilePath;
	ifstream removeFileCheck;
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++) {
			for (int l = 0; l < 7; l++)
			{
				backRootStreams[i][j][l]->close();
				removeFilePath = folderBase + "_" + to_string(i-3) + "_" + to_string(j-3) + "_" + to_string(l-3) + ".txt";
				removeFileCheck.open(removeFilePath);
				if (is_empty(removeFileCheck))
				{
					removeFileCheck.close();
					remove(removeFilePath.c_str());
				}
				else
				{
					cout << removeFilePath << endl;
					removeFileCheck.close();
				}
				
				
			}
		}
	}
}

bool multiFiles::is_empty(std::ifstream& pFile)
{
	return pFile.peek() == std::ifstream::traits_type::eof();
}
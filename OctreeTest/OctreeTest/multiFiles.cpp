#include "multiFiles.h"



multiFiles::multiFiles()
{
}


multiFiles::~multiFiles()
{
}
void multiFiles::initialBackRootFiles() {
	for (int i = 0; i < streamRange; i++)
	{
		for (int j = 0; j < streamRange; j++) {
			for (int l = 0; l < streamRange; l++)
			{
				sstm.str("");
				sstm << folderBase  <<"_"<< i- halfRange <<"_"<< j- halfRange << "_"<< l- halfRange <<".txt";
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
	if (dataFile.is_open())
	{
		cout << filePath << " Open Successful" << endl;
	}
	else {
		cerr << "Error: " << strerror(errno);
	}
	while (getline(dataFile, line))
	{
		counter++;
		istringstream iss(line);
		if (!(iss >> x >> y >> z >> r >> g >> b >> a)) 
		{ 
			cout << line << endl;
			cout << "File input error.\n";
			break; 
		} // error

		basePosition[0] = x / 100 ;
		basePosition[1] = y / 100 ;
		basePosition[2] = z / 100 ;
		if (basePosition[0] <-halfRange || basePosition[0] > halfRange || basePosition[1] <-halfRange || basePosition[1] > halfRange || basePosition[2] <-halfRange || basePosition[2] > halfRange)
		{
			sstm.str("");
			sstm << folderBase << "_" << basePosition[0]<< "_" << basePosition[1] << "_" << basePosition[2]<< ".txt";
			ofstream currentWriteFile(sstm.str());
			currentWriteFile << line+"\n";
			currentWriteFile.close();
		}
		else {
			*backRootStreams[basePosition[0] + halfRange][basePosition[1] + halfRange][basePosition[2] + halfRange] << line+"\n";
		}
	}
	dataFile.close();
}
void multiFiles::deleteEmptyFiles() {
	string removeFilePath;
	ifstream removeFileCheck;
	for (int i = 0; i < streamRange; i++)
	{
		for (int j = 0; j < streamRange; j++) {
			for (int l = 0; l < streamRange; l++)
			{
				backRootStreams[i][j][l]->close();
				removeFilePath = folderBase + "_" + to_string(i- halfRange) + "_" + to_string(j- halfRange) + "_" + to_string(l- halfRange) + ".txt";
				removeFileCheck.open(removeFilePath);
				if (is_empty(removeFileCheck))
				{
					removeFileCheck.close();
					remove(removeFilePath.c_str());
				}
				else
				{
					//cout << removeFilePath << endl;
					removeFileCheck.close();
				}
			}
		}
	}
}
void multiFiles::deleteAllFiles() {
	string removeFilePath;
	for (int i = 0; i < streamRange; i++)
	{
		for (int j = 0; j < streamRange; j++) {
			for (int l = 0; l < streamRange; l++)
			{
			//	cout << stderr;
				removeFilePath = folderBase + "_" + to_string(i - halfRange) + "_" + to_string(j - halfRange) + "_" + to_string(l - halfRange) + ".txt";
				remove(removeFilePath.c_str());

			}
		}
	}
}
bool multiFiles::is_empty(std::ifstream& pFile)
{
	return pFile.peek() == std::ifstream::traits_type::eof();
}
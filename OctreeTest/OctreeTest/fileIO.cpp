#include "fileIO.h"
fileIO::fileIO()
{
}


fileIO::~fileIO()
{
}

int fileIO::fileIn(string fileName) {
	ifstream dataFile;
	dataFile.open(fileName);
	string line;
	float counter = 0;
	float x, y, z, value, a, r, g, b;
	while (getline(dataFile, line))
	{
		vector<float> rowValue;
		istringstream iss(line);
		if (!(iss >> x >> y >> z >> r >> g >> b >> a)) { continue; } // error
		rowValue.push_back(x);
		rowValue.push_back(y);
		rowValue.push_back(z);
		rowValue.push_back(r);
		rowValue.push_back(g);
		rowValue.push_back(b);
		rowValue.push_back(a);
		fileValues.push_back(rowValue);
		counter++;
	}
	return counter;
}

vector<vector<float>> fileIO::getFileValues() {
	return fileValues;
}

int fileIO::fileOut(vector<vector<float>> fileValuesIn, string fileName) {
	ofstream dataFile;
	dataFile.open(fileName);
	int counter = 0;
	for (const auto& row : fileValuesIn)
	{
		counter++;
		//cout << counter << endl;
		copy(row.cbegin(), row.cend(), ostream_iterator<float>(dataFile, " "));
		dataFile << '\n';

	}
	return counter;
}
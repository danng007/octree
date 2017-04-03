#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include<math.h>
#include<vector>
#include<fstream>
#include<sstream>
#include<time.h>
#include<direct.h>
#include "writeBin.h"
#include "templateBin.h"
#include <thread>
using namespace std;
//Define Octree Class
template<class T>
struct OctreeNode
{
	T data; //root data
	T xmin, xmax; //root coordinate
	T ymin, ymax;
	T zmin, zmax;
	OctreeNode <T> *top_left_front, *top_left_back; // link to child
	OctreeNode <T> *top_right_front, *top_right_back;
	OctreeNode <T> *bottom_left_front, *bottom_left_back;
	OctreeNode <T> *bottom_right_front, *bottom_right_back;
	OctreeNode //node class
	(T nodeValue = T(),
		T xminValue = T(), T xmaxValue = T(),
		T yminValue = T(), T ymaxValue = T(),
		T zminValue = T(), T zmaxValue = T(),
		OctreeNode<T>* top_left_front_Node = NULL,
		OctreeNode<T>* top_left_back_Node = NULL,
		OctreeNode<T>* top_right_front_Node = NULL,
		OctreeNode<T>* top_right_back_Node = NULL,
		OctreeNode<T>* bottom_left_front_Node = NULL,
		OctreeNode<T>* bottom_left_back_Node = NULL,
		OctreeNode<T>* bottom_right_front_Node = NULL,
		OctreeNode<T>* bottom_right_back_Node = NULL)
		:data(nodeValue),
		xmin(xminValue), xmax(xmaxValue),
		ymin(yminValue), ymax(ymaxValue),
		zmin(zminValue), zmax(zmaxValue),
		top_left_front(top_left_front_Node),
		top_left_back(top_left_back_Node),
		top_right_front(top_right_front_Node),
		top_right_back(top_right_back_Node),
		bottom_left_front(bottom_left_front_Node),
		bottom_left_back(bottom_left_back_Node),
		bottom_right_front(bottom_right_front_Node),
		bottom_right_back(bottom_right_back_Node) {}
};
//Create Octree
template <class T>
void createOctree(OctreeNode<T> * &root, int maxdepth, double xmin, double xmax, double ymin, double ymax, double zmin, double zmax, int *indexCounter)
{
	//cout<<"Processing����"<<endl;
	maxdepth = maxdepth - 1; //depth-1 for one time's recursion
	if (maxdepth == 0) {
		root = new OctreeNode<T>();
		(*indexCounter)++;
		root->data = *indexCounter; //Give value to current root.
		root->xmin = xmin; //root's coordinate
		root->xmax = xmax;
		root->ymin = ymin;
		root->ymax = ymax;
		root->zmin = zmin;
		root->zmax = zmax;
		root->top_left_back = NULL;
		root->top_left_front = NULL;
		root->top_right_back = NULL;
		root->top_right_front = NULL;
		root->bottom_left_back = NULL;
		root->bottom_left_front = NULL;
		root->bottom_right_back = NULL;
		root->bottom_right_front = NULL;
	}
	if (maxdepth > 0) {
		root = new OctreeNode<T>();
		(*indexCounter)++;
		root->data = *indexCounter; //Give value to current root.
		root->xmin = xmin; //root's coordinate
		root->xmax = xmax;
		root->ymin = ymin;
		root->ymax = ymax;
		root->zmin = zmin;
		root->zmax = zmax;
		T xm = (xmax - xmin) / 2;//Half size for each edge
		T ym = (ymax - ymin) / 2;
		T zm = (zmax - zmin) / 2;
		//Recursively create child octree.
		createOctree(root->bottom_left_back, maxdepth, xmin, xmax - xm, ymin, ymax - ym, zmin, zmax - zm, indexCounter);
		createOctree(root->top_left_back, maxdepth, xmin, xmax - xm, ymin, ymax - ym, zmax - zm, zmax, indexCounter);
		createOctree(root->bottom_left_front, maxdepth, xmin, xmax - xm, ymax - ym, ymax, zmin, zmax - zm, indexCounter);
		createOctree(root->top_left_front, maxdepth, xmin, xmax - xm, ymax - ym, ymax, zmax - zm, zmax, indexCounter);
		createOctree(root->bottom_right_back, maxdepth, xmax - xm, xmax, ymin, ymax - ym, zmin, zmax - zm, indexCounter);
		createOctree(root->top_right_back, maxdepth, xmax - xm, xmax, ymin, ymax - ym, zmax - zm, zmax, indexCounter);
		createOctree(root->bottom_right_front, maxdepth, xmax - xm, xmax, ymax - ym, ymax, zmin, zmax - zm, indexCounter);
		createOctree(root->top_right_front, maxdepth, xmax - xm, xmax, ymax - ym, ymax, zmax - zm, zmax, indexCounter);

	}
}
int i = 1;
//Pre-order loop Octree
template <class T>
void preOrder(OctreeNode<T> * & p)
{
	if (p)
	{
		cout << "Current Point Value��" << p->data << "\nCoordinate��";
		cout << " xmin: " << p->xmin << " xmax: " << p->xmax;
		cout << " ymin: " << p->ymin << " ymax: " << p->ymax;
		cout << " zmin: " << p->zmin << " zmax: " << p->zmax;
		i += 1;
		cout << endl;

		preOrder(p->bottom_left_back);
		preOrder(p->top_left_back);
		preOrder(p->bottom_left_front);
		preOrder(p->top_left_front);
		preOrder(p->bottom_right_back);
		preOrder(p->top_right_back);
		preOrder(p->bottom_right_front);
		preOrder(p->top_right_front);
		cout << endl;
	}
}
//Depth of Octree
template<class T>
int depth(OctreeNode<T> *& p)
{
	if (p == NULL)
		return -1;
	int h = depth(p->top_left_front);
	return h + 1;
}
//Calculate unit length
int cal(int num)
{
	int result = 1;
	if (1 == num)
		result = 1;
	else
	{
		for (int i = 1; i<num; i++)
			result = 2 * result;
	}
	return result;
}
//Search Point
int times = 0;
static double xmin = 0, xmax = 0, ymin = 0, ymax = 0, zmin = 0, zmax = 0;
int tmaxdepth = 0;
float txm = 1, tym = 1, tzm = 1;
template<class T>
void find(OctreeNode<T> *& p, double x, double y, double z)
{
	int testt = 1;
	double xm = (p->xmax - p->xmin) / 2;
	double ym = (p->ymax - p->ymin) / 2;
	double zm = (p->zmax - p->zmin) / 2;
	times++;
	if (x>xmax || x<xmin || y>ymax || y<ymin || z>zmax || z<zmin)
	{
		cout << "The point is not in scene��" << endl;
		return;
	}
	if (x <= p->xmin + txm && x >= p->xmax - txm && y <= p->ymin + tym && y >= p->ymax - tym && z <= p->zmin + tzm && z >= p->zmax - tzm)
	{
		cout << endl << "Find Point��" << "Located:" << endl;
		cout << "Point Index: " << p->data << endl;
		cout << " xmin: " << p->xmin << " xmax: " << p->xmax;
		cout << " ymin: " << p->ymin << " ymax: " << p->ymax;
		cout << " zmin: " << p->zmin << " zmax: " << p->zmax;
		cout << "\nBlock." << endl;
		cout << "After " << times << " Recursion��" << endl;
	}
	else if (x <= (p->xmax - xm) && y <= (p->ymax - ym) && z <= (p->zmax - zm))
	{
		cout << "Walk through block��" << endl;
		cout << "Point Index: " << p->data << endl;
		cout << " xmin: " << p->xmin << " xmax: " << p->xmax;
		cout << " ymin: " << p->ymin << " ymax: " << p->ymax;
		cout << " zmin: " << p->zmin << " zmax: " << p->zmax;
		cout << endl;
		find(p->bottom_left_back, x, y, z);
	}
	else if (x<(p->xmax - xm) && y<(p->ymax - ym) && z>(p->zmax - zm))
	{
		cout << "Walk through block��" << endl;
		cout << "Point Index: " << p->data << endl;
		cout << " xmin: " << p->xmin << " xmax: " << p->xmax;
		cout << " ymin: " << p->ymin << " ymax: " << p->ymax;
		cout << " zmin: " << p->zmin << " zmax: " << p->zmax;
		cout << endl;
		find(p->top_left_back, x, y, z);
	}
	else if (x>(p->xmax - xm) && y<(p->ymax - ym) && z<(p->zmax - zm))
	{
		cout << "Walk through block��" << endl;
		cout << "Point Index: " << p->data << endl;
		cout << " xmin: " << p->xmin << " xmax: " << p->xmax;
		cout << " ymin: " << p->ymin << " ymax: " << p->ymax;
		cout << " zmin: " << p->zmin << " zmax: " << p->zmax;
		cout << endl;
		find(p->bottom_right_back, x, y, z);
	}
	else if (x>(p->xmax - xm) && y<(p->ymax - ym) && z>(p->zmax - zm))
	{
		cout << "Walk through block��" << endl;
		cout << "Point Index: " << p->data << endl;
		cout << " xmin: " << p->xmin << " xmax: " << p->xmax;
		cout << " ymin: " << p->ymin << " ymax: " << p->ymax;
		cout << " zmin: " << p->zmin << " zmax: " << p->zmax;
		cout << endl;
		find(p->top_right_back, x, y, z);
	}
	else if (x<(p->xmax - xm) && y>(p->ymax - ym) && z<(p->zmax - zm))
	{
		cout << "Walk through block��" << endl;
		cout << "Point Index: " << p->data << endl;
		cout << " xmin: " << p->xmin << " xmax: " << p->xmax;
		cout << " ymin: " << p->ymin << " ymax: " << p->ymax;
		cout << " zmin: " << p->zmin << " zmax: " << p->zmax;
		cout << endl;
		find(p->bottom_left_front, x, y, z);
	}
	else if (x<(p->xmax - xm) && y>(p->ymax - ym) && z>(p->zmax - zm))
	{
		cout << "Walk through block��" << endl;
		cout << "Point Index: " << p->data << endl;
		cout << " xmin: " << p->xmin << " xmax: " << p->xmax;
		cout << " ymin: " << p->ymin << " ymax: " << p->ymax;
		cout << " zmin: " << p->zmin << " zmax: " << p->zmax;
		cout << endl;
		find(p->top_left_front, x, y, z);
	}
	else if (x>(p->xmax - xm) && y>(p->ymax - ym) && z<(p->zmax - zm))
	{
		cout << "Walk through block��" << endl;
		cout << "Point Index: " << p->data << endl;
		cout << " xmin: " << p->xmin << " xmax: " << p->xmax;
		cout << " ymin: " << p->ymin << " ymax: " << p->ymax;
		cout << " zmin: " << p->zmin << " zmax: " << p->zmax;
		cout << endl;
		find(p->bottom_right_front, x, y, z);
	}
	else if (x>(p->xmax - xm) && y>(p->ymax - ym) && z>(p->zmax - zm))
	{
		cout << "Walk through block��" << endl;
		cout << "Point Index: " << p->data << endl;
		cout << " xmin: " << p->xmin << " xmax: " << p->xmax;
		cout << " ymin: " << p->ymin << " ymax: " << p->ymax;
		cout << " zmin: " << p->zmin << " zmax: " << p->zmax;
		cout << endl;
		find(p->top_right_front, x, y, z);
	}
}
void calIndex(float x, float y, float z, vector<int> levels, vector<long long int> index, vector<long long int> totalIndex, int maxdepth, ofstream &outputFile)
{

	float midX = (xmax - xmin) / 2 + xmin;
	float midY = (ymax - ymin) / 2 + ymin;
	float midZ = (zmax - zmin) / 2 + zmin;
	//outputFile << "levels 0: " << levels[0] << " Index in tree:" << levels[0] << endl;
	//Write first level into file
	string dest = "./dataFiles/";
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
		//outputFile << levels[i] << " " << index[i] << endl; 
		cout << "Levels" << i << " In: " << levels[i] << " Index: " << index[i] << endl;
		dest += to_string(levels[i]);
		//uncommand this one, full data will write into file

	}
	cout << dest << endl;
	_mkdir(dest.c_str());
	ofstream fout;
	fout.open(dest, ios::out | ios::binary);
	outputFile << levels[maxdepth - 1] << " " << index[maxdepth - 1] << endl;
	//this line will write only last level into file
}
void initialIndex(vector<long long int> &index, vector<int> &levels, vector<long long int> &totalIndex, int maxdepth) {

	totalIndex[maxdepth - 1] = 1;
	index[maxdepth - 1] = 0;
	levels[maxdepth - 1] = 8;
	for (int i = 1; i < maxdepth; i++)
	{
		index[i] = 0;
		levels[i] = 8;
		totalIndex[maxdepth - 1 - i] = pow(8, i) + totalIndex[maxdepth - i];
	}
}
void readBin() {
	cout << "Read bin" << endl;
	ifstream *binFile;
	binFile = new ifstream("./dataFiles/r07656.bin", ios::in | ios::binary);
	int c;

	while (binFile->good()) {
		cout << "Processing Read bin" << endl;
		char* buffer = new char[16];
		binFile->read(buffer, 16); //1 size byte for 2 number
		int* iBuffer = reinterpret_cast<int*>(buffer);
		cout << iBuffer[0] << endl;
		cout << iBuffer[1] << endl;
		cout << iBuffer[2] << endl;
		unsigned char* ucBuffer = reinterpret_cast<unsigned char*>(buffer + 12);
		cout << float(ucBuffer[0]) << endl;
		cout << float(ucBuffer[1]) << endl;
		cout << float(ucBuffer[2]) << endl;
		cout << float(ucBuffer[3]) << endl;
	}
	cout << "Finish Read bin" << endl;
}
//main
void threadRun(string dataFilePath, vector<int> levels, vector<long long int> index, vector<long long int> totalIndex, int maxdepth) {
	double timeLast;
	double timeBegin;
	string line;
	float x, y, z, value, a;
	int r, g, b;
	ifstream dataFile;
	dataFile.open(dataFilePath);
	templateBin *tb = new templateBin(xmin, xmax, ymin, ymax, zmin, zmax, 0.01, maxdepth);
	timeBegin = clock();
	int counter = 0;

	while (getline(dataFile, line))
	{
		counter++;
		//cout << "Inside" << endl;
		istringstream iss(line);

		if (!(iss >> x >> y >> z >> r >> g >> b >> a)) { break; } // error
		a = a * 100;
		tb->calIndex(x, y, z, r, g, b, a, levels, index, totalIndex);
	}


	tb->createAllHrc();

	timeLast = (clock() - timeBegin)*1.0 / CLOCKS_PER_SEC;
	cout << "*************************************" << endl;
	cout << "Single Thread Finish Processing Time: " << timeLast << " s" << endl;
	cout << "Processing Points: "<<counter << endl;
	cout << "*************************************" << endl;
}
int main()
{
	double timeLast;
	double timeBegin;
	//readBin();
	int maxdepth = 0;
	int indexCounter = 0;
	cout << "Input Max Depth:" << endl;
	cin >> maxdepth;
	vector<long long>index(maxdepth, 1);
	vector<long long>totalIndex(maxdepth, 1);
	vector<int>levels(maxdepth, 1);

	cout << "Input Range of Size\nInput Order:xmin,xmax,ymin,ymax,zmin,zmax" << endl;
	cin >> xmin >> xmax >> ymin >> ymax >> zmin >> zmax;
	initialIndex(index, levels, totalIndex, maxdepth);

	
	string path1 = "./dataFiles/thretest1.txt";
	string path2 = "./dataFiles/thretest2.txt";
	timeBegin = clock();
	thread t1(threadRun, path1, levels, index, totalIndex, maxdepth);
	thread t2(threadRun, path2, levels, index, totalIndex, maxdepth);
	t1.join();
	t2.join();
	timeLast = (clock() - timeBegin)*1.0 / CLOCKS_PER_SEC;
	cout << "*************************************" << endl;
	cout << "In Total Finish Processing Time: " << timeLast << " s" << endl;
	cout << "*************************************" << endl;
	int endx;
	cin >> endx;



	//OctreeNode<float> * rootNode = NULL;
	//int choiced = 0;
	//while (true)
	//{
	//	system("cls");
	//	cout << "Options:\n";
	//	cout << "1.Create Octree\n2.Pre-order Octree\n";
	//	cout << "3.Check Tree Depth\n4.Search Point in Tree   \n";
	//	cout << "5.Self Calculation Test\n";
	//	cout << "0.Exit\n\n";
	//	cin >> choiced;
	//	if (choiced == 0)
	//		return 0;
	//	else if (choiced == 1)
	//	{
	//		system("cls");
	//		cout << "Input Max Depth:" << endl;
	//		cin >> maxdepth;

	//		cout << "Input Range of Size. Input Order:xmin,xmax,ymin,ymax,zmin,zmax" << endl;
	//		cin >> xmin >> xmax >> ymin >> ymax >> zmin >> zmax;
	//		if (maxdepth >= 0 || xmax>xmin || ymax>ymin || zmax>zmin || xmin>0 || ymin>0 || zmin>0)
	//		{
	//			tmaxdepth = cal(maxdepth);
	//			txm = (xmax - xmin) / tmaxdepth;
	//			tym = (ymax - ymin) / tmaxdepth;
	//			tzm = (zmax - zmin) / tmaxdepth;
	//			//createOctree(rootNode,maxdepth,xmin,xmax,ymin,ymax,zmin,zmax,&indexCounter);
	//		}
	//		else
	//		{
	//			cout << "Wrong Input";
	//			return 0;
	//		}
	//	}
	//	else if (choiced == 2)
	//	{
	//		system("cls");
	//		cout << "Pre-order print Octree\n";
	//		i = 1;
	//		preOrder(rootNode);
	//		cout << endl;
	//		system("pause");
	//	}
	//	else if (choiced == 3)
	//	{
	//		system("cls");
	//		int dep = depth(rootNode);
	//		cout << "Depth of the Tree:\n" << dep + 1 << endl;
	//		system("pause");
	//	}
	//	else if (choiced == 4)
	//	{
	//		system("cls");
	//		cout << "Input search point coordinate, order:x,y,z\n";
	//		double x, y, z;
	//		cin >> x >> y >> z;
	//		times = 0;
	//		cout << endl << "Search Processing..." << endl;
	//		find(rootNode, x, y, z);
	//		system("pause");
	//	}
	//	else if (choiced == 5) {
	//		system("cls");
	//		cout << "Input search point coordinate, order:x,y,z\n";
	//		double x, y, z;
	//		cin >> x >> y >> z;
	//		times = 0;
	//		calIndex(x, y, z, levels, index, totalIndex, maxdepth, outputFile);

	//		system("pause");
	//	}
	//	else
	//	{
	//		system("cls");
	//		cout << "\n\nWrong Choice\n";
	//		system("pause");
	//	}
	//}
}
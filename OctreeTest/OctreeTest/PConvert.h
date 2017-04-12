#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include "Point.h"
#include "pointReader.h"
#include "potreeWriter.h"
using std::vector;
using std::string;

class PCconvert
{
public:
	PCconvert();
	~PCconvert();
	void convert();
	void prepare();
};


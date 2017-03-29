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

#include "Point.h"
#include "boost/assign.hpp"
#include "boost/algorithm/string.hpp"

using namespace boost::assign;
using boost::split;
using boost::token_compress_on;
using boost::is_any_of;
using boost::trim;
using boost::erase_all;
using namespace std;

namespace Potree {
	class pointReader
	{
	private:
		int pointsRead;
		ifstream stream;
		Point point;
		float colorOffset;
		float colorScale;
		float intensityOffset;
		float intensityScale;
	public:
		pointReader(string path);
		Point getPoint();
		bool readNextPoint();
		~pointReader();
	};


}

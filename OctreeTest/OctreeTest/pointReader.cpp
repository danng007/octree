//#include "pointReader.h"
//
//
//namespace Potree {
//	pointReader::pointReader(string path)
//	{
//		stream = ifstream(path, std::ios::in | std::ios::binary);
//		pointsRead = 0;
//		colorScale = 255.0;
//		intensityScale = 1.0;
//	}
//
//
//	pointReader::~pointReader()
//	{
//	}
//
//	bool pointReader::readNextPoint() {
//		double x = 0;
//		double y = 0;
//		double z = 0;
//		unsigned char r = 255;
//		unsigned char g = 255;
//		unsigned char b = 255;
//		unsigned short intensity = 0;
//		string line;
//		while (getline(stream, line)) {
//			trim(line);
//			vector<string> tokens;
//			split(tokens, line, is_any_of("\t ,"), token_compress_on);
//			int i = 0;
//			x = stod(tokens[0]);
//			y = stod(tokens[1]);
//			z = stod(tokens[2]);
//			r = (unsigned char)(255.0f * (stof(tokens[3])) / colorScale);
//			g = (unsigned char)(255.0f * (stof(tokens[4])) / colorScale);
//			b = (unsigned char)(255.0f * (stof(tokens[5])) / colorScale);
//			intensity = (unsigned short)(65535 * (stof(tokens[6])) / intensityScale);
//			point = Point(x, y, z, r, g, b, intensity);
//			pointsRead++;
//			return true;
//		}
//
//		return false;
//	}
//	Point pointReader::getPoint() {
//		return point;
//	}
//}

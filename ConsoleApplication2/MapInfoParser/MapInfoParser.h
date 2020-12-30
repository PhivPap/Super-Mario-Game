#pragma once
#include <string>
#include <map>
#include "Util.h"


class MapInfoParser{
	std::map<std::string, std::string> map_info;

public:
	MapInfoParser(const char* file_name);
	std::string GetMapInfoStr(std::string);
	double GetMapInfoDouble(std::string);
	uint GetMapInfoUint(std::string);
	int GetMapInfoInt(std::string);
	Rect GetMapInfoRect(std::string);
	Point GetMapInfoPoint(std::string);
};


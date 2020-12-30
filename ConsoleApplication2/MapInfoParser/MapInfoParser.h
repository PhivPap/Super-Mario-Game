#pragma once
#include <string>
#include <map>
#include "Util.h"


class MapInfoParser{
	std::map<std::string, std::string> map_info;

public:
	MapInfoParser();
	void SetNewParser(const char*);
	std::string GetStr(std::string);
	double GetDouble(std::string);
	uint GetUint(std::string);
	int GetInt(std::string);
	Rect GetRect(std::string);
	Point GetPoint(std::string);
};


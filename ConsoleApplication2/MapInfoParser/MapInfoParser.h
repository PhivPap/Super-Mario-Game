#pragma once
#include <string>
#include <map>

class MapInfoParser{
	std::map<std::string, std::string> map_info;

public:
	MapInfoParser(std::string);
	std::string GetMapInfoStr(std::string);
	double GetMapInfoDouble(std::string);
	unsigned int GetMapInfoUint(std::string);
	int GetMapInfoInt(std::string);
};


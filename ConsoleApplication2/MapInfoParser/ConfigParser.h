#pragma once
#include <string>
#include <map>
#include <vector>
#include "Types.h"


class ConfigParser{
	std::map<std::string, std::string> map_info;

public:
	ConfigParser();
	void SetNewParser(const char*);
	std::string GetStr(std::string);
	double GetDouble(std::string);
	uint GetUint(std::string);
	int GetInt(std::string);
	Rect GetRect(std::string);
	Point GetPoint(std::string);
	std::vector<std::string> GetList(std::string);
};


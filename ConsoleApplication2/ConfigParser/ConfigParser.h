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
	std::string GetStr(const std::string&);
	double GetDouble(const std::string&);
	uint GetUint(const std::string&);
	int GetInt(const std::string&);
	Rect GetRect(const std::string&);
	Point GetPoint(const std::string&);
	Dim GetDim(const std::string&);
	std::vector<std::string> GetList(const std::string&);
	std::vector<int> GetListInt(const std::string&);
};


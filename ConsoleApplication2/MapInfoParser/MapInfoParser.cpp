#include "MapInfoParser.h"
#include <fstream>
#include <iostream>
#include <vector>

MapInfoParser::MapInfoParser() {

}

static void splitLineWithChar(std::vector<std::string>& split_line, std::string line, char c) {
	std::string item;
	for (unsigned int i = 0; i < line.length(); i++) {
		if (line[i] == c) {
			split_line.push_back(item);
			item.clear();
			continue;
		}
		item += line[i];
	}
	if (item != "")
		split_line.push_back(item);
}


void MapInfoParser::SetNewParser(const char* file_name) {
	map_info.clear();
	std::ifstream map_info_file(file_name);
	if (!map_info_file.is_open()) {
		std::cerr << "Could not open map info file: '" << file_name << "'\n";
		exit(1);
	}
	std::string line;
	std::string var;
	std::string value;
	while (getline(map_info_file, line)) {
		if (line == "")
			continue;
		auto i = line.find_first_of(':');
		if (i == std::string::npos || i == 0) {
			std::cerr << "Care: map info file line: '" << line << "' didnt parse.\n";
			continue;
		}
		var = line.substr(0, i);
		value = line.substr(i + 1);
		map_info.insert(std::pair<std::string, std::string>(var,value));
	}
	map_info_file.close();

}

// !!CARE <><><><> REF?
std::string MapInfoParser::GetStr(std::string key) {
	auto info = map_info.find(key);
	if (info == map_info.end()) {
		std::cerr << "No map info element with name: '" << key << "'\n";
		exit(1);
	}
	return info->second;
}

double MapInfoParser::GetDouble(std::string key) {
	auto info_str = GetStr(key);
	double info_dbl = std::stod(info_str); // throws exeption if cant convert	
	return info_dbl;
}

unsigned int MapInfoParser::GetUint(std::string key) {
	auto info_str = GetStr(key);
	unsigned int info_uint = std::stoul(info_str); // throws exeption if cant convert	
	return info_uint;
}

int MapInfoParser::GetInt(std::string key) {
	auto info_str = GetStr(key);
	int info_int = std::stoi(info_str); // throws exeption if cant convert	
	return info_int;
}

Rect MapInfoParser::GetRect(std::string key) {
	auto info_str = GetStr(key);
	std::vector<std::string> values;
	splitLineWithChar(values, info_str, ',');
	if (values.size() < 4) {
		std::cerr << "Could not get Rect from '" << info_str << "'\n";
		exit(1);
	}
	return { std::stoul(values[0]), std::stoul(values[1]), std::stoul(values[2]), std::stoul(values[3])}; // gonna throw exept if cant convert
}

Point MapInfoParser::GetPoint(std::string key) {
	auto info_str = GetStr(key);
	std::vector<std::string> values;
	splitLineWithChar(values, info_str, ',');
	if (values.size() < 2) {
		std::cerr << "Could not get Point from '" << info_str << "'\n";
		exit(1);
	}
	return { std::stoul(values[0]), std::stoul(values[1]) };
}
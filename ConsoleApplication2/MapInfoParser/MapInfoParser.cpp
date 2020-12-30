#include "MapInfoParser.h"
#include <fstream>
#include <iostream>


MapInfoParser::MapInfoParser(std::string file_name) {
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
		std::cout << var << " - " << value << std::endl;
		// fill map.
		/*	style of file:
		* 
		* VARIABLE0:VALUE0
		* VARIABLE1:VALUE1
		*
		* 
		*/
	}
	map_info_file.close();

}

// !!CARE <><><><> REF?
std::string MapInfoParser::GetMapInfoStr(std::string key) {
	auto info = map_info.find(key);
	if (info == map_info.end()) {
		std::cerr << "No map info element with name: '" << key << "'\n";
		exit(1);
	}
	return info->second;
}

double MapInfoParser::GetMapInfoDouble(std::string key) {
	//auto info = to_double(GetMapInfoStr(key));
	return 0.0f;
}

unsigned int MapInfoParser::GetMapInfoUint(std::string key) {
	//auto info = to_uint(GetMapInfoStr(key));
	return 0;
}

int MapInfoParser::GetMapInfoInt(std::string key) {
	//auto info = to_int(GetMapInfoStr(key));
	return 0;
}
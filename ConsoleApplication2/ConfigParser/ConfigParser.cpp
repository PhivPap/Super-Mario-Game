#include "ConfigParser.h"
#include <fstream>
#include <iostream>

ConfigParser::ConfigParser() {
}

//str is modified
static void ReplaceSubStrings(std::string& str, std::string replace_this, std::string replace_with) {
	size_t i = 0;
	auto sub_str_len = replace_this.length();
	if (sub_str_len == 0)
		return;
	while (true) {
		i = str.find(replace_this, i);
		if (i == std::string::npos)
			break;
		str.replace(i, sub_str_len, replace_with);
	}
}

static void SplitLineWithChar(std::vector<std::string>& split_line, std::string line, char c) {
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


void ConfigParser::SetNewParser(const char* file_name) {
	map_info.clear();
	std::ifstream map_info_file(file_name);
	if (!map_info_file.is_open()) {
		std::cerr << "Could not open config file: '" << file_name << "'\n";
		exit(1);
	}
	std::string line;
	std::string var;
	std::string value;
	while (getline(map_info_file, line)) {
		auto comment_idx = line.find("##");
		if (comment_idx != std::string::npos) {
			line = line.substr(0, comment_idx);
		}
		ReplaceSubStrings(line, " ", "");
		if(line == "")
			continue;
		auto i = line.find_first_of(':');
		if (i == std::string::npos || i == 0) {
			std::cerr << "Care: config file line: '" << line << "' didnt parse.\n";
			continue;
		}
		var = line.substr(0, i);
		value = line.substr(i + 1);
		map_info.insert(std::pair<std::string, std::string>(var,value));
	}
	map_info_file.close();

}

// !!CARE <><><><> REF?
std::string ConfigParser::GetStr(std::string key) {
	auto info = map_info.find(key);
	if (info == map_info.end()) {
		std::cerr << "No config file element with name: '" << key << "'\n";
		exit(1);
	}
	return info->second;
}

double ConfigParser::GetDouble(std::string key) {
	auto info_str = GetStr(key);
	double info_dbl = std::stod(info_str); // throws exeption if cant convert	
	return info_dbl;
}

unsigned int ConfigParser::GetUint(std::string key) {
	auto info_str = GetStr(key);
	unsigned int info_uint = std::stoul(info_str); // throws exeption if cant convert	
	return info_uint;
}

int ConfigParser::GetInt(std::string key) {
	auto info_str = GetStr(key);
	int info_int = std::stoi(info_str); // throws exeption if cant convert	
	return info_int;
}

Rect ConfigParser::GetRect(std::string key) {
	auto info_str = GetStr(key);
	std::vector<std::string> values;
	SplitLineWithChar(values, info_str, ',');
	if (values.size() < 4) {
		std::cerr << "Could not get Rect from '" << info_str << "'\n";
		exit(1);
	}
	return { std::stoul(values[0]), std::stoul(values[1]), std::stoul(values[2]), std::stoul(values[3])}; // gonna throw exept if cant convert
}

Point ConfigParser::GetPoint(std::string key) {
	auto info_str = GetStr(key);
	std::vector<std::string> values;
	SplitLineWithChar(values, info_str, ',');
	if (values.size() < 2) {
		std::cerr << "Could not get Point from '" << info_str << "'\n";
		exit(1);
	}
	return { std::stoul(values[0]), std::stoul(values[1]) };
}

/* Returns value*/
std::vector<std::string> ConfigParser::GetList(std::string key) {
	auto info_str = GetStr(key);
	std::vector<std::string> values;
	SplitLineWithChar(values, info_str, ',');
	return values;
}
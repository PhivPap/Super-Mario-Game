#pragma once
#include "Types.h"
#include <string>

class Pipe {
protected:
	std::string scene;
public:
	Pipe(std::string&);
	const std::string& GetScene() const;

};
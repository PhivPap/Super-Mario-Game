#pragma once
#include "Pipe.h"

class VPipe : Pipe {
private:
	uint x0, x1, y;
public:
	VPipe(std::string&, uint, uint, uint);
	bool RectCollidesWithPipe(const Rect&);
};


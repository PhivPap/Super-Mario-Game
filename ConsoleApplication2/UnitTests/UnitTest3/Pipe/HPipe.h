#pragma once
#include "Pipe.h"

class HPipe : Pipe {
private:
	uint y0, y1, x;
public:
	HPipe(std::string&, uint, uint, uint);
	bool RectCollidesWithPipe(const Rect&);
};


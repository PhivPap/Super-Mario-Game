#pragma once
#include "Types.h"
#include <functional>

class MotionQuantizer{

public:
	using Mover = std::function<void(const Rect&, int&, int&)>;
protected:
	int horiz_max = 0, vert_max = 0;
	Mover mover;
	bool used = false;
public:
	MotionQuantizer& SetRange(int, int);
	MotionQuantizer& SetMover(Mover&);
	void Move(const Rect&, int&, int&);
};


#pragma once
#include "MovingAnimation.h"

class FrameRangeAnimation : public MovingAnimation {
protected:
	uint start = 0, end = 0;

public:
	using Me = FrameRangeAnimation;
	uint		GetStartFrame(void) const;
	Me&			SetStartFrame(uint v);
	uint		GetEndFrame(void) const;
	Me&			SetEndFrame(uint v);
	Animation*	Clone(void) const;
	FrameRangeAnimation(const std::string&,uint, uint, uint r, int, int, int);
};



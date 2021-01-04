#include "FrameRangeAnimation.h"

uint FrameRangeAnimation::GetStartFrame(void) const {
	return start;
}

FrameRangeAnimation& FrameRangeAnimation::SetStartFrame(uint v) {
	start = v;
	return *this;
}

uint FrameRangeAnimation::GetEndFrame(void) const {
	return end;
}

FrameRangeAnimation& FrameRangeAnimation::SetEndFrame(uint v) {
	end = v;
	return *this;
}

Animation* FrameRangeAnimation::Clone(void) const {
	return new FrameRangeAnimation(id, start, end, GetReps(), GetDx(), GetDy(), GetDelay());
}

FrameRangeAnimation::FrameRangeAnimation(const std::string& _id, uint s, uint e, uint r, int dx, int dy, int d)
	: start(s), end(e), MovingAnimation(id, r, dx, dy, d) {}

FrameRangeAnimation::~FrameRangeAnimation(void) {}
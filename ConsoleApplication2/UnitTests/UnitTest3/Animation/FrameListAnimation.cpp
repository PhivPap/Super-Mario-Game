#include "FrameListAnimation.h"

using Frames = std::vector<uint>;

FrameListAnimation::~FrameListAnimation(void) {}

FrameListAnimation::FrameListAnimation(const std::string& _id, const Frames& _frames, uint r, int dx, int dy, uint d)
	: frames(_frames), MovingAnimation(id, r, dx, dy, d) {}

const Frames& FrameListAnimation::GetFrames(void) const {
	return frames;
}

void FrameListAnimation::SetFrames(const Frames& f) {
	frames = f;
}

Animation* FrameListAnimation::Clone(void) const {
	return new FrameListAnimation(id, frames, GetReps(), GetDx(), GetDy(), GetDelay());
} 

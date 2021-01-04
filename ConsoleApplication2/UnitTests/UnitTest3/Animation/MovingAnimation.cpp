#include "MovingAnimation.h"

MovingAnimation::MovingAnimation(const std::string& _id, uint _reps, int _dx, int _dy, uint _delay)
	: Animation(_id), reps(_reps), dx(_dx), dy(_dy), delay(_delay) {}

int MovingAnimation::GetDx(void) const {
	return dx;
}

MovingAnimation& MovingAnimation::SetDx(int v) {
	dx = v;
	return *this;
}

int MovingAnimation::GetDy(void) const {
	return dy;
}

MovingAnimation& MovingAnimation::SetDy(int v) {
	dy = v;
	return *this;
}

uint MovingAnimation::GetDelay(void) const {
	return delay;
}

MovingAnimation& MovingAnimation::SetDelay(uint v) {
	delay = v;
	return *this;
}

uint MovingAnimation::GetReps(void) const {
	return reps;
}

MovingAnimation& MovingAnimation::SetReps(uint n) {
	reps = n;
	return *this;
}

bool MovingAnimation::IsForever(void) const {
	return !reps; // 0
}

MovingAnimation& MovingAnimation::SetForever(void) {
	reps = 0;
	return *this;
}

Animation* MovingAnimation::Clone(void) const {
	return new MovingAnimation(id, reps, dx, dy, delay);
}
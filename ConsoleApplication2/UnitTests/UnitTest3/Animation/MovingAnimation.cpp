#include "MovingAnimation.h"

MovingAnimation::~MovingAnimation(void) {}

MovingAnimation::MovingAnimation(const std::string& _id, uint _reps, const Velocity& _v, uint _delay)
	: Animation(_id), reps(_reps), velocity(_v), delay(_delay) {}

const Velocity& MovingAnimation::GetVelocity(void) const {
	return velocity;
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
	return new MovingAnimation(id, reps, velocity, delay);
}
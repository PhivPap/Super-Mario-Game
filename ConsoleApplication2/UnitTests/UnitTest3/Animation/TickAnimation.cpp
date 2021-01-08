#include "TickAnimation.h"
#include <assert.h>

TickAnimation::TickAnimation (const std::string& _id, uint d, uint r, bool discrete)
	: Animation(id), delay(d), reps(r), isDiscrete(discrete) { /*assert(Inv());*/ }

bool TickAnimation::Inv (void) const {
	return isDiscrete || reps == 1;
}

uint TickAnimation::GetDelay (void) const {
 return delay;
 }

TickAnimation& TickAnimation::SetDelay (uint v) {
	delay = v;
	return *this;
}

uint TickAnimation::GetReps (void) const {
	return reps;
}

TickAnimation& TickAnimation::SetReps (uint n) {
	reps = n;
	return *this;
}

bool TickAnimation::IsForever (void) const {
	return !reps;
}

TickAnimation& TickAnimation::SetForever (void) {
	reps = 0;
	return *this;
}

bool TickAnimation::IsDiscrete (void) const {
	return isDiscrete;
}

Animation* TickAnimation::Clone (void) const {
	return new TickAnimation(id, delay, reps, isDiscrete);
}
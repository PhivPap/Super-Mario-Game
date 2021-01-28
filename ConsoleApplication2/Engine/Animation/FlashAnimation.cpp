#include "FlashAnimation.h"

FlashAnimation& FlashAnimation::SetReps(uint n) {
	repetitions = n;
	return *this;
}

uint FlashAnimation::GetReps(void) const {
	return repetitions;
}

FlashAnimation& FlashAnimation::SetHideDelay(uint d) {
	hideDelay = d;
	return *this;
}

uint FlashAnimation::GetHideDelay(void) const {
	return hideDelay;
}

FlashAnimation& FlashAnimation::SetShowDelay(uint d) {
	showDelay = d;
	return *this;
}

uint FlashAnimation::GetShowDelay(void) const {
	return showDelay;
}

Animation* FlashAnimation::Clone(void) const {
	return new FlashAnimation(id, repetitions, hideDelay, showDelay);
}

FlashAnimation::FlashAnimation(const std::string& _id, uint n, uint show, uint hide) :
		Animation(id), repetitions(n), hideDelay(hide), showDelay(show) {}
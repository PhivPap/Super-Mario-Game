#include "MovingAnimator.h"

MovingAnimator::MovingAnimator(Sprite* sprite) : Animator(sprite) {}

void MovingAnimator::Progress(timestamp_t currTime) {
	while (currTime > lastTime && (currTime - lastTime) >= anim->GetDelay()) { // approx will run 1 time for fast game loops
		lastTime += anim->GetDelay();
		NotifyAction(*anim);
		if (!anim->IsForever() && ++currRep == anim->GetReps()) {
			state = ANIMSTATE_T::ANIMATOR_FINISHED;
			NotifyStopped();
			return;
		}
	}
}

const MovingAnimation&  MovingAnimator::GetAnim(void) const {
	return *anim;
}

void MovingAnimator::Start(MovingAnimation* a, timestamp_t t) {
	anim = a;
	lastTime = t;
	state = ANIMSTATE_T::ANIMATOR_RUNNING;
	currRep = 0;
	NotifyStarted();
}
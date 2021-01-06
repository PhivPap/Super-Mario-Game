#include "FrameRangeAnimator.h"

FrameRangeAnimator::FrameRangeAnimator(Sprite* sprite) : Animator(sprite) {}

void FrameRangeAnimator::Progress(timestamp_t currTime) {
	while (currTime > lastTime && (currTime - lastTime) >= anim->GetDelay()) {
		if (currFrame == anim->GetEndFrame()) {
			assert(anim->IsForever() || currRep < anim->GetReps());
			currFrame = anim->GetStartFrame(); // flip to start
		}
		else
			++currFrame;
		sprite->SetFrame(currFrame);
		lastTime += anim->GetDelay();
		NotifyAction(*anim);
		if (currFrame == anim->GetEndFrame())
			if (!anim->IsForever() && ++currRep == anim->GetReps()) {
				state = ANIMSTATE_T::ANIMATOR_FINISHED;
				NotifyStopped();
				return;
			}
	}
}

uint FrameRangeAnimator::GetCurrFrame(void) const {
	return currFrame;
}

uint FrameRangeAnimator::GetCurrRep(void) const {
	return currRep;
}

void FrameRangeAnimator::Start(FrameRangeAnimation* a, timestamp_t t) {
	anim = a;
	lastTime = t;
	state = ANIMSTATE_T::ANIMATOR_RUNNING;
	currFrame = anim->GetStartFrame();
	currRep = 0;
	NotifyStarted();
	NotifyAction(*anim);
}

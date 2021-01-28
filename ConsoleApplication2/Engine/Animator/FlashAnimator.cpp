#include "FlashAnimator.h"

void FlashAnimator::Progress(timestamp_t currTime) {

//	while (currTime > lastTime && (currTime - lastTime) >= anim->GetHideDelay() + anim->GetShowDelay()) {
//		lastTime += anim->GetHideDelay() + anim->GetShowDelay();
//		NotifyAction(*anim);
//		
//		if (!anim->IsForever() && ++currRep == anim->GetReps()) {
//			state = ANIMATORSTATE_T::ANIMATOR_FINISHED;
//			NotifyStopped();
//			return;
//		}
//	}
}

const FlashAnimation& FlashAnimator::GetAnim(void) const {
	return *anim;
}

void FlashAnimator::Start(FlashAnimation* a, timestamp_t t) {
	anim = a;
	lastTime = t;
	state = ANIMATORSTATE_T::ANIMATOR_RUNNING;
	currRep = 0;
	NotifyStarted(*anim);
	NotifyAction(*anim);
}
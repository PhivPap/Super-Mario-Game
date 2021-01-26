#include "TickAnimator.h"

 
void TickAnimator::Progress (timestamp_t currTime) {
	if (!anim->IsDiscrete()) {
		elapsedTime = currTime - lastTime;
		lastTime = currTime;
		NotifyAction(*anim);
		return;
	}

	while (currTime > lastTime && (currTime - lastTime) >= anim->GetDelay()) {
		
		lastTime += anim->GetDelay();
		NotifyAction(*anim);
	
		if (!anim->IsForever() && ++currRep == anim->GetReps()) {
			state = ANIMATORSTATE_T::ANIMATOR_FINISHED;
			NotifyStopped();
			return;
		}
	}
}
 
uint TickAnimator::GetCurrRep (void) const {
	return currRep;
}
 
uint TickAnimator::GetElapsedTime (void) const {
	return elapsedTime;
}
 
float TickAnimator::GetElapsedTimeNormalised (void) const {
	return float(elapsedTime) / float(anim->GetDelay());
}
 
void TickAnimator::Start(const TickAnimation& a, timestamp_t t) {
	anim = (TickAnimation*)a.Clone();
	lastTime = t;
	state = ANIMATORSTATE_T::ANIMATOR_RUNNING;
	currRep = 0;
	elapsedTime = 0;
	NotifyStarted(*anim);
}
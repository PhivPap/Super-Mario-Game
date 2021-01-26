#include "FrameListAnimator.h"

void FrameListAnimator::Progress(timestamp_t currTime) {
	auto frames_len =anim->GetFrames().size() - 1;
	while (currTime > lastTime && (currTime - lastTime) >= anim->GetDelay()) {
		if (frame_no_idx == frames_len) {
			////assert(anim->IsForever() || currRep < anim->GetReps());
			frame_no_idx = 0; // flip to start
		}
		else
			++frame_no_idx;

		lastTime += anim->GetDelay();
		NotifyAction(*anim);
		if (frame_no_idx == frames_len && !anim->IsForever()) {
			//if (!anim->IsForever() && ++currRep == anim->GetReps()) {
			state = ANIMATORSTATE_T::ANIMATOR_FINISHED;
			NotifyStopped();
			return;
		}
	}
}

uint FrameListAnimator::GetCurrFrame(void) const {
	return anim->GetFrames()[frame_no_idx];
}

void FrameListAnimator::Start(FrameListAnimation* a, timestamp_t t) {
	anim = a;
	lastTime = t;
	state = ANIMATORSTATE_T::ANIMATOR_RUNNING;
	frame_no_idx = 0;
	NotifyStarted(*anim);
	NotifyAction(*anim);
}

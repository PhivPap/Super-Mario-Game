#include "Animator.h"
#include "AnimatorManager.h"

Animator::Animator(void) : state(ANIMATORSTATE_T::ANIMATOR_STOPPED){
	AnimatorManager::GetSingleton().Register(this);
}

Animator::~Animator(void) {
	AnimatorManager::GetSingleton().Cancel(this);
}

void Animator::Finish(bool isForced) {
	if (!HasFinished()) {
		state = isForced ? ANIMATORSTATE_T::ANIMATOR_STOPPED : ANIMATORSTATE_T::ANIMATOR_FINISHED;
		NotifyStopped();
	}
}

void Animator::Stop(void) {
	Finish(true);
}

void Animator::NotifyStopped(void) {
	AnimatorManager::GetSingleton().MarkAsSuspended(this);
	if (onFinish)
		(onFinish)(this);
}

void Animator::NotifyStarted(const Animation& anim){
	AnimatorManager::GetSingleton().MarkAsRunning(this);
	if (onStart)
		(onStart)(this, anim);
}

void Animator::NotifyAction(const Animation& anim) {
	if (onAction)
		(onAction)(this, anim);
}

void Animator::TimeShift(timestamp_t offset) {
	lastTime += offset;
}

bool Animator::HasFinished(void) const {
	return state != ANIMATORSTATE_T::ANIMATOR_RUNNING;
}

 void Animator::SetOnFinish(const OnFinish& f) {
	onFinish = f;
}

void Animator::SetOnStart(const OnStart& f) {
	onStart = f;
}

void Animator::SetOnAction(const OnAction& f) {
	onAction = f;
}
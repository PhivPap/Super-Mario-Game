#pragma once
#include "TickAnimation.h"
#include "Animator.h"

class TickAnimator : public Animator {
protected:
	TickAnimation*	anim = nullptr;
	uint			currRep = 0;
	uint			elapsedTime = 0;

public:
	void			Progress (timestamp_t);
	uint			GetCurrRep (void) const;
	uint			GetElapsedTime(void) const; 
	float			GetElapsedTimeNormalised(void) const;
	void			Start(const TickAnimation&, timestamp_t);
	TickAnimator (void) = default;
};

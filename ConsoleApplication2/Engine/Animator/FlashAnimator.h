#pragma once
#include "Animator.h"
#include "FlashAnimation.h"

class FlashAnimator : public Animator {
protected:
	FlashAnimation* anim = nullptr;
	uint			currRep = 0;

public:
	void					Progress(timestamp_t);
	const FlashAnimation&	GetAnim(void) const;
	void					Start(FlashAnimation*, timestamp_t);
	FlashAnimator(void) = default;
};


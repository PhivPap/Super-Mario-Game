#pragma once
#include "Animator.h"
#include "MovingAnimation.h"

class MovingAnimator : public Animator {
protected:
	MovingAnimation*		anim = nullptr;
	uint					currRep = 0; // animation state

public:
	void					Progress(timestamp_t);
	const MovingAnimation&	GetAnim(void) const;
	void					Start(MovingAnimation*, timestamp_t);
	MovingAnimator(void) = default;
	MovingAnimator(Sprite*);
};


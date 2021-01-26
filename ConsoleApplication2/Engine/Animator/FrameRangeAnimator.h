#include "Animator.h"
#include "FrameRangeAnimation.h"
#include "Types.h"
#include <assert.h>

class FrameRangeAnimator : public Animator {
protected:
	FrameRangeAnimation*	anim = nullptr;
	uint					currFrame = 0; // animation state
	uint					currRep = 0; // animation state

public:
	void Progress(timestamp_t);
	uint GetCurrFrame(void) const;
	uint GetCurrRep(void) const;
	void Start(FrameRangeAnimation*, timestamp_t);
	FrameRangeAnimator(void) = default;
};
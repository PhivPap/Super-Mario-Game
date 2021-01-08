#include "Animator.h"
#include "FrameListAnimation.h"
#include "Types.h"

class FrameListAnimator : public Animator {
private:
	FrameListAnimation* anim = nullptr;
	uint				frame_no_idx = 0;
	//uint				currRep = 0; // animation state

public:
	void Progress(timestamp_t);
	uint GetCurrFrame(void) const;
	//uint GetCurrRep(void) const;
	void Start(FrameListAnimation*, timestamp_t);
	FrameListAnimator(void) = default;
};

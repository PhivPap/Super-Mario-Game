#pragma once
#include "Animation.h"
#include "Types.h"

class FlashAnimation : public Animation {
private:
	uint repetitions = 0;
	uint hideDelay = 0;
	uint showDelay = 0;
public:
	using Me = FlashAnimation;
	Me&			SetRepetitions(uint n);
	uint		GetRepetitions(void) const;
	Me&			SetHideDelay(uint);
	uint		GetHideDelay(void) const;
	Me&			SetShowDelay(uint);
	uint		GetShowDelay(void) const;
	Animation*	Clone(void) const override;
	FlashAnimation(const std::string&, uint, uint, uint);
};
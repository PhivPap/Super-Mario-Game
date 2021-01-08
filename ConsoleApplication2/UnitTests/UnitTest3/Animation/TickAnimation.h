#pragma once
#include "Animation.h"
#include "Types.h"

class TickAnimation : public Animation {
protected:
	uint delay = 0;
	uint reps = 1;
	bool isDiscrete = true;
	bool Inv(void) const;

public:
	using Me = TickAnimation;
	uint		GetDelay(void) const;
	Me&			SetDelay(uint);
	uint		GetReps(void) const;
	Me&			SetReps(uint);
	bool		IsForever(void) const;
	Me&			SetForever(void);
	bool		IsDiscrete(void) const;
	Animation*	Clone(void) const override;
	TickAnimation(const std::string&, uint, uint, bool);
};
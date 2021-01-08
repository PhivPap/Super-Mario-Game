#pragma once
#include "Types.h"
#include "Animation.h"

class MovingAnimation : public Animation {
protected:
	uint	reps = 1; // 0=forever
	//int		dx = 0, dy = 0;
	Velocity velocity;
	uint	delay = 0;

public:
	using Me = MovingAnimation;
	const Velocity& GetVelocity(void) const;
	uint		GetDelay(void) const;
	Me&			SetDelay(uint);
	uint		GetReps(void) const;
	Me&			SetReps(uint);
	bool		IsForever(void) const;
	Me&			SetForever(void);
	Animation*	Clone(void) const;
	MovingAnimation(const std::string&, uint, const Velocity&, uint);
	~MovingAnimation(void);
};


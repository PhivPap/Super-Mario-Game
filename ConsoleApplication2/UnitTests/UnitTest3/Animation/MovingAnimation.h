#pragma once
#include "Types.h"
#include "Animation.h"
class MovingAnimation : public Animation {
protected:
	uint	reps = 1; // 0=forever
	int		dx = 0, dy = 0;
	uint	delay = 0;

public:
	using Me = MovingAnimation;
	int			GetDx(void) const;
	Me&			SetDx(int);
	int			GetDy(void) const;
	Me&			SetDy(int);
	uint		GetDelay(void) const;
	Me&			SetDelay(uint);
	uint		GetReps(void) const;
	Me&			SetReps(uint);
	bool		IsForever(void) const;
	Me&			SetForever(void);
	Animation*	Clone(void) const;
	MovingAnimation(const std::string&, uint, int, int, uint);
	~MovingAnimation(void);
};


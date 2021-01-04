#pragma once
#include <vector>
#include "MovingAnimation.h"

class FrameListAnimation : public MovingAnimation {
public:
	using Frames = std::vector<uint>;

protected:
	Frames frames;

public:
	const Frames&	GetFrames(void) const;
	void			SetFrames(const Frames&);
	Animation*		Clone(void) const;
	FrameListAnimation(const std::string&, const Frames&, uint, int, int, uint);
};
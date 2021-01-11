#pragma once
#include <functional>
#include "Types.h"

class GravityHandler{
public:
	using OnSolidGroundPred =	std::function<bool(const Rect_f&)>;
	using OnStartFalling =		std::function<void(void)>;
	using OnStopFalling =		std::function<void(void)>;

protected:
	bool				gravity_addicted = false;
	bool				is_falling = false;
	OnSolidGroundPred	on_solid_ground;
	OnStartFalling		on_start_falling;
	OnStopFalling		on_stop_falling;

public:
	void SetOnSolidGround(const OnSolidGroundPred&);
	void SetOnStartFalling(const OnStartFalling&);
	void SetOnStopFalling(const OnStopFalling&);
	void Reset(void);
	void Check(const Rect_f& r);
	void SetGravity();
	void RemoveGravity();
};


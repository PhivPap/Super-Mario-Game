#include "GravityHandler.h"

void GravityHandler::SetOnSolidGround(const OnSolidGroundPred& on_solid_ground) {
	this->on_solid_ground = on_solid_ground;
}

void GravityHandler::SetOnStartFalling(const OnStartFalling& on_start_falling) {
	this->on_start_falling = on_start_falling;
}

void GravityHandler::SetOnStopFalling(const OnStopFalling& on_stop_falling) {
	this->on_stop_falling = on_stop_falling;
}

void GravityHandler::Reset(void) {
	is_falling = false;
}

void GravityHandler::Check(const Rect_f& r) {
	if (gravity_addicted) {
		if (on_solid_ground(r)) {
			if (is_falling) {
				is_falling = false;
				on_stop_falling();
			}
		}
		else {
			if (!is_falling) {
				is_falling = true;
				on_start_falling();
			}
		}
	}
}

void GravityHandler::SetGravity() {
	gravity_addicted = true;
}

void GravityHandler::RemoveGravity() {
	gravity_addicted = false;
}
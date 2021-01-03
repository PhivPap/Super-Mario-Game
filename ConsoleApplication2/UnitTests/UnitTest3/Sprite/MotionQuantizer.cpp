#include "MotionQuantizer.h"

template <typename Tnum>
int number_sign(Tnum x) { return x > 0 ? 1 : x < 0 ? -1 : 0; }


MotionQuantizer& MotionQuantizer::SetRange(int h, int v) {
	horiz_max = h;
	vert_max = v;
	used = true;
	return *this;
}
MotionQuantizer& MotionQuantizer::SetMover(Mover& mover) {
	this->mover = mover;
	return *this;
}

void MotionQuantizer::Move(const Rect& r, int& dx, int& dy) { // takes ref??
	if (!used)
		mover(r, dx, dy);
	else {
		do {
			auto dx_final = std::min(number_sign(dx) * horiz_max, dx);
			auto dy_final = std::min(number_sign(dy) * vert_max, dy);

			mover(r, dx_final, dy_final);

			if (!dx_final) // X motion denied
				dx = 0;
			else
				dx -= dx_final;

			if (!dy_final) // Y motion denied
				dy = 0;
			else
				dy -= dy_final;

		} while (dx || dy);
	}
}
#include "Clipper.h"

template <class T> bool clip_rect(
	T x, T y, T w, T h,
	T wx, T wy, T ww, T wh,
	T& cx, T& cy, T& cw, T& ch ) {

	cw = T(std::min(wx + ww, x + w)) - (cx = T(std::max(wx, x)));
	ch = T(std::min(wy + wh, y + h)) - (cy = T(std::max(wy, y)));
	return *cw > 0 && *ch > 0;
}

static inline bool clip_rect(const Rect& r, const Rect& area, Rect& result) {
	return clip_rect(	
		r.x , r.y, r.w, r.h, 
		area.x, area.y, area.w, area.h, 
		result.x, result.y, result.w, result.h
	);
}

Clipper& Clipper::SetView(View& view) {
	this->view = view;
	return *this;
}

bool Clipper::Clip(const Rect& r, const Rect& dpy_area, Point* dpy_pos, Rect* clipped_box) const {
	Rect visible_area;
	if (!clip_rect(r, view(), visible_area)) {
		clipped_box->w = clipped_box->h = 0;
		return false;
	}
	else {
		clipped_box->x = r.x - visible_area.x;
		clipped_box->y = r.y - visible_area.y;
		clipped_box->w = visible_area.w;
		clipped_box->h = visible_area.h;

		dpy_pos->x = dpy_area.x + (visible_area.x - view().x);
		dpy_pos->y = dpy_area.y + (visible_area.y - view().y);

		return true;
	}

}
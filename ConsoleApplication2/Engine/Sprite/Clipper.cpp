#include "Clipper.h"

Rect* Clipper::view_window = 0;

template <typename T> bool clip_rect(
	T x, T y, T w, T h,
	T wx, T wy, T ww, T wh,
	int& cx, int& cy, int& cw, int& ch ) {

	cw = T(std::min(wx + ww, x + w)) - (cx = T(std::max(wx, x)));
	ch = T(std::min(wy + wh, y + h)) - (cy = T(std::max(wy, y)));
	return cw > 0 && ch > 0;
}

static inline bool clip_rect(const Rect& r, const Rect* area, Rect_i& result) {
	return clip_rect(
		r.x , r.y, r.w, r.h,
		area->x, area->y, area->w, area->h,
		result.x, result.y, result.w, result.h
	);
}

//Clipper& Clipper::SetView(View& view) {
//	this->view = view;
//	return *this;
//}

void Clipper::InitViewWindow(Rect* _view_window){
	view_window = _view_window;
}

bool Clipper::Clip(const Rect& r, const Rect& dpy_area, Point* dpy_pos, Rect_i* clipped_box) const {
	Rect_i visible_area;
	if (!clip_rect(r, view_window, visible_area)) {
		clipped_box->w = clipped_box->h = 0;
		return false;
	}
	else {
		clipped_box->x = visible_area.x - r.x;
		clipped_box->y = visible_area.y - r.y;
		clipped_box->w = visible_area.w;
		clipped_box->h = visible_area.h;

		dpy_pos->x = 0 + (visible_area.x - view_window->x);
		dpy_pos->y = 0 + (visible_area.y - view_window->y);

		return true;
	}
}
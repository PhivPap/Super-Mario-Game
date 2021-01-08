#include "Sprite.h"
#include "SpriteManager.h"

template <typename Tnum>
static inline int number_sign(Tnum x) { return x > 0 ? 1 : x < 0 ? -1 : 0; }

Sprite::Sprite(int x, int y, const AnimationFilm* film, const std::string& type_id = "")
	: x(x), y(y), curr_film(film), type_id(type_id)
{
	frame_no = curr_film->GetTotalFrames(); 
	SetFrame(0);

	SpriteManager::GetSingleton().Add(this);
}

Sprite::~Sprite() {
	// do things here?
	SpriteManager::GetSingleton().Remove(this);
}

void Sprite::SetState(const std::string& _state_id) {
	state_id = _state_id;
}

const std::string& Sprite::GetState(void) {
	return state_id;
}

void Sprite::QuantizerSetRange(int h, int w) {
	horiz_max = h;
	vert_max = w;
	quantizer_used = true;
}

void Sprite::SetMover(const Mover& f) {
	mover = f;
	//quantizer.SetMover(mover = f); // <-- ... ok dude
}

const Rect_f Sprite::GetBoxF(void) const {
	return { x, y, (float)frame_box.w, (float)frame_box.h };
}

const Rect Sprite::GetBox(void) const {
	return { (uint)x, (uint)y, frame_box.w, frame_box.h };
}

void Sprite::GetPos(float& _x, float& _y){
	_x = x;
	_y = y;
}

void Sprite::Move(int dx, int dy) {
	assert(0);
	//auto r = GetBox();
	//if (!quantizer_used)
	//	mover(r, dx, dy);
	//else {
	//	do {
	//		auto dx_final = std::min(number_sign(dx) * horiz_max, dx);
	//		auto dy_final = std::min(number_sign(dy) * vert_max, dy);

	//		mover(r, dx_final, dy_final);

	//		if (!dx_final) // X motion denied
	//			dx = 0;
	//		else
	//			dx -= dx_final;

	//		if (!dy_final) // Y motion denied
	//			dy = 0;
	//		else
	//			dy -= dy_final;

	//	} while (dx || dy);
	//}
}

void Sprite::SetPos(float x, float y) {
	this->x = x;
	this->y = y;
}

void Sprite::SetZorder(uint z) {
	z_order = z;
}

uint Sprite::GetZorder(void) {
	return z_order;
}

void Sprite::SetFrame(byte i) {
	if (i != frame_no) {
		assert(i < curr_film->GetTotalFrames());
		frame_box = curr_film->GetFrameBox(frame_no = i);
	}
}

byte Sprite::GetFrame(void) const {
	return frame_no;
}

void Sprite::SetBoundingArea(const BoundingArea& area) {
	assert(!bounding_area); 
	bounding_area = area.Clone();
}

void Sprite::SetBoundingArea(BoundingArea* area) {
	assert(!bounding_area);
	bounding_area = area;
}

const BoundingArea* Sprite::GetBoundArea(void) const {
	return bounding_area;
}

const std::string& Sprite::GetTypeId(void) {
	return type_id;
}

void Sprite::SetVisibility(bool v) {
	is_visible = v;
}

bool Sprite::isVisible(void) const {
	return is_visible;
}

bool Sprite::CollisionCheck(const Sprite*) const {
	assert(0);
	return false;
	//TODO
}

void Sprite::Display(ALLEGRO_BITMAP* dest, const Rect& dpy_area, const Clipper& clipper) const {
	Rect_i clipped_box;
	Point dpy_pos;
	if (clipper.Clip(GetBox(), dpy_area, &dpy_pos, &clipped_box)) {
		Rect clipped_frame{
			frame_box.x + clipped_box.x,
			frame_box.y + clipped_box.y,
			clipped_box.w,
			clipped_box.h
		};
		curr_film->DisplayFrame(dest, dpy_pos, clipped_frame);
	}
}

void Sprite::SetVelocity(const Velocity& _velocity) {
	velocity = _velocity;
}

const Velocity& Sprite::GetVelocity(void) {
	return velocity;
}
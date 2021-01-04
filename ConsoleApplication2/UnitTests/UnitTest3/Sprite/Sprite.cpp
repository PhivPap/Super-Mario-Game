#include "Sprite.h"
#include "SpriteManager.h"

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

void Sprite::SetMover(const Mover& f) {
	quantizer.SetMover(mover = f); // <-- ... ok dude
}

const Rect Sprite::GetBox(void) const {
	return { (uint)x, (uint)y, frame_box.w, frame_box.h }; // do we need x,y ??
}

void Sprite::Move(int dx, int dy) {
	quantizer.Move(GetBox(), dx, dy);
}

void Sprite::SetPos(int x, int y) {
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

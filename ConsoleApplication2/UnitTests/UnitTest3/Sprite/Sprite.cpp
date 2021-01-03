#include "Sprite.h"

void Sprite::SetMover(const Mover& f) {
	quantizer.SetMover(mover = f); // <-- ... ok dude
}

const Rect Sprite::GetBox(void) const {
	return { x, y, frame_box.w, frame_box.h }; // do we need x,y ??
}

void Sprite::Move(int dx, int dy) {
	quantizer.Move(GetBox(), &dx, &dy); // why pass pointer?
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
	//TODO
}
void Sprite::Display(ALLEGRO_BITMAP, const Rect&, const Clipper&) const {
	assert(0);
	//TODO
}

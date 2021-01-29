#include "Sprite.h"
#include "SpriteManager.h"
#include "BoundingArea/BoundingBox.h"

template <typename Tnum>
static inline int number_sign(Tnum x) { return x > 0 ? 1 : x < 0 ? -1 : 0; }

Sprite::Sprite(int x, int y, const AnimationFilm* film, const std::string& type_id = "")
	: x(x), y(y), curr_film(film), type_id(type_id){

	SetZorder(100);
	frame_no = curr_film->GetTotalFrames(); 
	SetFrame(0);
	
	SpriteManager::GetSingleton().Add(this);

	SetBoundingArea(new BoundingBox(uint(x), uint(y), x + frame_box.w, y + frame_box.h));
	uniform_box.w = frame_box.w;
	uniform_box.h = frame_box.h;
}

Sprite::Sprite(int x, int y, const AnimationFilm* film, uint z, const std::string& type_id = "")
	: x(x), y(y), curr_film(film), z_order(z), type_id(type_id){
	
	//SetZorder(z);
	frame_no = curr_film->GetTotalFrames();
	SetFrame(0);

	SpriteManager::GetSingleton().Add(this);

	SetBoundingArea(new BoundingBox(uint(x), uint(y), x + frame_box.w, y + frame_box.h));
	uniform_box.w = frame_box.w;
	uniform_box.h = frame_box.h;
}

Sprite::~Sprite() {
	// do things here?
	delete bounding_area;
	SpriteManager::GetSingleton().Remove(this);
}


void Sprite::SetMovingSprite(bool moving_sprite){
	this->moving_sprite = moving_sprite;
}

bool Sprite::IsMovingSprite(){
	return moving_sprite;
}

void Sprite::SetTypeId(const std::string& _type_id) {
	type_id = _type_id;
}

const std::string& Sprite::GetTypeId(void) {
	return type_id;
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
	return { x, y, (float)uniform_box.w, (float)uniform_box.h };
}

const Rect Sprite::GetBox(void) const {
	return { (uint)x, (uint)y, uniform_box.w, uniform_box.h };
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

void Sprite::ResetBoundingArea(const BoundingArea& area) {
	assert(bounding_area);
	delete bounding_area;
	bounding_area = area.Clone();
}

void Sprite::ResetBoudingArea(BoundingArea* area) {
	assert(bounding_area);
	delete bounding_area;
	bounding_area = area;
}

const BoundingArea* Sprite::GetBoundArea(void) {
	if (update_bound_area_pos) {
		bounding_area->UpdatePos(uint(x), uint(y)); // 200iq syntax
		update_bound_area_pos = false;
	}
	return bounding_area;
}

void Sprite::SetVisibility(bool v) {
	is_visible = v;
}

bool Sprite::isVisible(void) const {
	return is_visible;
}

bool Sprite::CollisionCheck(Sprite* sprite) {
	return GetBoundArea()->Intersects(*sprite->GetBoundArea());
	//auto* my_bound_area = GetBoundArea();
	//auto* his_bound_area = sprite->GetBoundArea();

	//assert(0);
	//return false;
	//TODO
}

void Sprite::Display(ALLEGRO_BITMAP* dest, const Rect& dpy_area, const Clipper& clipper) const {
	Rect_i clipped_box;
	Point dpy_pos;
	if (!is_visible)
		return;
	if (clipper.Clip({uint(x), uint(y), frame_box.w, frame_box.h}, dpy_area, &dpy_pos, &clipped_box)) {
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

GravityHandler& Sprite::GetGravityHandler(void) {
	return gravity;
}

void Sprite::SetHasDirectMotion(bool direct_motion) {
	this->direct_motion = direct_motion;
}

bool Sprite::GetHasDirectMotion(void) const {
	return direct_motion;
}

void Sprite::SetUpdateBoundAreaPos(bool update_bound_area_pos) {
	this->update_bound_area_pos = update_bound_area_pos;
}

void Sprite::SetUniformBox(const Dim& uniform_box) {
	this->uniform_box = uniform_box;
	ResetBoudingArea(new BoundingBox(uint(x), uint(y), x + uniform_box.w, y + uniform_box.h));
}

const Dim& Sprite::GetUniformBox() {
	return uniform_box;
}
#include "AnimationFilm.h"

AnimationFilm::AnimationFilm(const std::string& _id) : id(_id) { assert(0); } // why we want this tho ?

AnimationFilm::AnimationFilm(ALLEGRO_BITMAP* _bitmap, const std::vector<Rect>& _boxes, const std::string& _id)
	: bitmap(_bitmap), boxes(_boxes), id(_id)
{
	al_convert_mask_to_alpha(bitmap, al_map_rgb(255, 0, 255)); // set transparent color for all animation film bitmaps
}

byte AnimationFilm::GetTotalFrames(void) const {
	return boxes.size();
}

ALLEGRO_BITMAP* AnimationFilm::GetBitmap(void) const {
	return bitmap;
}

std::string AnimationFilm::GetId(void) const {
	return id;
}

const Rect& AnimationFilm::GetFrameBox(byte frameNo) const {
	assert(boxes.size() > frameNo);
	return boxes[frameNo];
}

void AnimationFilm::DisplayFrame(ALLEGRO_BITMAP* dest, const Point& at, byte frameNo) const {
	assert(0);
	//al_set_target_bitmap(dest);
	Rect src = GetFrameBox(frameNo);
	al_draw_bitmap_region(bitmap, src.x, src.y, src.w, src.h, at.x, at.y, 0);
	// who should flip ?
}

void AnimationFilm::DisplayFrame(ALLEGRO_BITMAP* dest, const Point& at, const Rect& src) const {
	//al_set_target_bitmap(dest);
	al_draw_bitmap_region(bitmap, src.x, src.y, src.w, src.h, at.x, at.y, 0);
}

void AnimationFilm::SetBitmap(ALLEGRO_BITMAP* b) {
	assert(!bitmap);
	bitmap = b;
}

void AnimationFilm::Append(const Rect& r) {
	boxes.push_back(r);
}
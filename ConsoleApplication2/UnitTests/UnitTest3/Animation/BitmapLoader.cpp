#include "BitmapLoader.h"
#include <iostream>

BitmapLoader::BitmapLoader(void) {}

BitmapLoader::~BitmapLoader() { CleanUp(); }

ALLEGRO_BITMAP* BitmapLoader::GetBitmap(const std::string& path) const {
	auto i = bitmaps.find(path);
	return i != bitmaps.end() ? i->second : nullptr;
}

ALLEGRO_BITMAP* BitmapLoader::Load(const std::string& path) {
	auto b = GetBitmap(path);
	if (!b) {
		std::cout << "loading bitmap from : \"" << path.c_str() << "\"" <<  std::endl;
		bitmaps[path] = b = al_load_bitmap(path.c_str());
		assert(b);
	}
	return b;
}
// prefer to massively clear bitmaps at the end than
// to destroy individual bitmaps during gameplay
void BitmapLoader::CleanUp(void) {
	for (auto& i : bitmaps)
		al_destroy_bitmap(i.second);
	bitmaps.clear();
}
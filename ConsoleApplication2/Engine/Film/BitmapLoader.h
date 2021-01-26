#pragma once
#include <map>
#include <string>
#include <assert.h>
#include <allegro5/allegro5.h>

class BitmapLoader {

	using Bitmaps = std::map<std::string, ALLEGRO_BITMAP*>;
	Bitmaps bitmaps;
	ALLEGRO_BITMAP* GetBitmap(const std::string& path) const;

public:
	ALLEGRO_BITMAP* Load(const std::string& path);
	void CleanUp(void);
	BitmapLoader(void);
	~BitmapLoader(void);
};

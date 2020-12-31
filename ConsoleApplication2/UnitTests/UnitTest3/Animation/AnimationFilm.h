#pragma once
#include "Level1.h"

class AnimationFilm {
	std::vector<Rect> boxes;
	ALLEGRO_BITMAP* bitmap;
	std::string id;

public:
	byte GetTotalFrames(void) const;
	ALLEGRO_BITMAP* GetBitmap(void) const;
	std::string GetId(void) const;
	const Rect& GetFrameBox(byte) const;
	void DisplayFrame(ALLEGRO_BITMAP*, const Point&, byte) const;
	void SetBitmap(ALLEGRO_BITMAP*);
	void Append(const Rect&);
	AnimationFilm(const std::string&);
	AnimationFilm(ALLEGRO_BITMAP*, const std::vector<Rect>&, const std::string&);
};
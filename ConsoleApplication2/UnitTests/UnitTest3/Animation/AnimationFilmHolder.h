#pragma once
#include <map>
#include <string>
#include "AnimationFilm.h"

class AnimationFilmHolder {
	using Films = std::map<std::string, AnimationFilm*>;
	Films films;
	//BitmapLoader bitmaps; // only for loading of film bitmaps
	static AnimationFilmHolder holder; // singleton
	AnimationFilmHolder(void);
	~AnimationFilmHolder();

public:
	static const AnimationFilmHolder& Get(void);
	void LoadAll(const std::string&);
	void CleanUp(void);
	const AnimationFilm* GetFilm(const std::string&) const;
};
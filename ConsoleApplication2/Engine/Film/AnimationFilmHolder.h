#pragma once
#include <map>
#include <string>
#include <list>
#include "BitmapLoader.h"
#include "AnimationFilm.h"

class AnimationFilmHolder {
public:
	using Parser = std::function<
		void(std::list <AnimationFilm::Data>& output, const char* input)
	>;

private:
	using Films = std::map<std::string, AnimationFilm*>;
	Films			films;
	BitmapLoader	bitmaps; // only for loading of film bitmaps
	static AnimationFilmHolder	holder; // singleton
	AnimationFilmHolder(void);
	~AnimationFilmHolder();

public:
	static inline AnimationFilmHolder& Get(void) { return holder; }
	void LoadAll(const char*, const Parser&); 
	void CleanUp(void);
	const AnimationFilm* GetFilm(const std::string&) const;
	const BitmapLoader& GetBitmapLoader(void) const;
};
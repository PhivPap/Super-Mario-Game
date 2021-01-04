#pragma once
#include "Level2.h"
#include "AnimationFilmHolder.h"

class UnitTest3 : protected UnitTest2 {


	std::function<void(void)> mario_physics;
	void SpriteLoader(void);
	void LoadSpriteList(std::vector<std::string>&, const AnimationFilm*, const std::string&);

public:
	UnitTest3();
	virtual void Initialise(void);
	virtual void Load(void);
	virtual void Clear(void);
	void Main();
};
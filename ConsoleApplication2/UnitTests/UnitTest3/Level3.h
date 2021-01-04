#pragma once
#include "Level2.h"
#include "AnimationFilmHolder.h"
#include "SpriteManager.h"

class UnitTest3 : protected UnitTest2 {

	std::function<void(void)> mario_physics;
	std::function<void(void)> display_sprites;
	SpriteManager& sprite_manager;
	Clipper default_clipper;
	void SpriteLoader(void);
	void LoadSpriteList(std::vector<std::string>&, const AnimationFilm*, const std::string&);

public:
	UnitTest3();
	virtual void Initialise(void);
	virtual void Load(void);
	virtual void Clear(void);
	void Main();
};
#pragma once
#include "Level2.h"
#include "AnimationFilmHolder.h"
#include "SpriteManager.h"
#include "Sprite.h"
#include "FrameRangeAnimation.h"
#include "FrameRangeAnimator.h"
#include "AnimatorManager.h"

class UnitTest3 : protected UnitTest2 {

	std::function<void(void)> mario_physics;
	std::function<void(void)> display_sprites;
	std::function<void(void)> animator_refresh;

	SpriteManager&		sprite_manager;
	AnimatorManager&	animator_manager;
	Clipper				default_clipper;
	void				SpriteLoader(void);
	void				LoadSpriteList(std::vector<std::string>&, const AnimationFilm*, const std::string&);

public:
	UnitTest3();
	virtual void Initialise(void);
	virtual void Load(void);
	virtual void Clear(void);
	void Main();
};
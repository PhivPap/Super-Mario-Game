#pragma once
#include "Level2.h"
#include "AnimationFilmHolder.h"
#include "SpriteManager.h"
#include "Sprite.h"
#include "FrameRangeAnimation.h"
#include "FrameRangeAnimator.h"
#include "FrameListAnimation.h"
#include "FrameListAnimator.h"
#include "AnimatorManager.h"
#include "TickAnimation.h"
#include "TickAnimator.h"

class UnitTest3 : protected UnitTest2 {

	std::function<void(void)> mario_physics;
	std::function<void(void)> display_sprites;
	std::function<void(void)> animator_refresh;

	std::list<Sprite*> moving_sprites;

	SpriteManager&		sprite_manager;
	AnimatorManager&	animator_manager;
	Clipper				default_clipper;
	void				SpriteLoader(void);
	std::list<Sprite*> 	LoadSpriteList(std::vector<std::string>&, const AnimationFilm*, const std::string&);

public:
	UnitTest3();
	virtual void Initialise(void);
	virtual void Load(void);
	virtual void Clear(void);
	void Main();
};
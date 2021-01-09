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
#include "AnimationHolder.h"
#include "CollisionChecker.h"


// TODO get from config file
#define FILMS_DATA_PATH			"UnitTests/UnitTest3/media/films.data"
#define ANIMS_FRAME_RANGE_PATH	"UnitTests/UnitTest3/media/Anims/anim_range.data"
#define ANIMS_FRAME_LIST_PATH	"UnitTests/UnitTest3/media/Anims/anim_list.data"
#define ANIMS_FRAME_TICK_PATH	"UnitTests/UnitTest3/media/Anims/anim_tick.data"

class UnitTest3 : protected UnitTest2 {

	std::function<void(void)> mario_physics;
	std::function<void(void)> display_sprites;
	std::function<void(void)> animator_refresh;

	std::list<Sprite*> moving_sprites;

	CollisionChecker&	collision_checker;
	SpriteManager&		sprite_manager;
	AnimatorManager&	animator_manager;
	Clipper				default_clipper;
	void				SpriteLoader(void);
	std::list<Sprite*> 	LoadSpriteList(std::vector<std::string>&, const AnimationFilm*, const std::string&, const std::string&);

public:
	UnitTest3();
	virtual void Initialise(void);
	virtual void Load(void);
	virtual void Clear(void);
	void Main();
};
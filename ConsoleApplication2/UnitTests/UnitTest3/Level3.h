#pragma once
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
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
#define ANIMS_TICK_PATH	"UnitTests/UnitTest3/media/Anims/anim_tick.data"

#define MAIN_CONFIG_PATH "UnitTests/UnitTest3/media/main_config.data"

class UnitTest3 : protected UnitTest2 {
private:
	std::function<void(void)> rect_movement;
	std::function<void(void)> display_sprites;
	std::function<void(void)> display_texts;
	std::function<void(void)> animator_refresh;
	std::function<void(void)> input_mario;

	std::list<Sprite*> moving_sprites;

	CollisionChecker&	collision_checker;
	SpriteManager&		sprite_manager;
	AnimatorManager&	animator_manager;
	Clipper				default_clipper;
	ConfigParser		main_config;
	ALLEGRO_FONT*		font0;
	ALLEGRO_COLOR		font0_color;

	uint				score = 0;
	uint				coins = 0;
	std::string			map_id;
	uint				time_left; // seconds.
	uint				lives;
	bool				time_is_up = false;

	double				max_fall_speed;
	double				g_acceleration;
	Sprite*				mario;
	TickAnimator		mario_jump_cd;

	void				SpriteLoader(void);
	//static bool			OnSolidGround(const Rect&);
	std::list<Sprite*> 	LoadSpriteList(std::vector<std::string>&, const AnimationFilm*, const std::string&, const std::string&);

	void CreateMario();
	void SetMarioGravity(Sprite* sprite, TickAnimation* fall_update = (TickAnimation*)AnimationHolder::Get().GetAnimation("FALL_UPDATE"));
	void SetDefaultGravity(Sprite* sprite, TickAnimation* fall_update = (TickAnimation*)AnimationHolder::Get().GetAnimation("FALL_UPDATE"));

public:
	UnitTest3();
	virtual void Initialise(void);
	virtual void Load(void);
	virtual void Clear(void);
	void Main();
};
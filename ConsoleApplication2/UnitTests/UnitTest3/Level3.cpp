#include "Level3.h"
#include "Util.h"
#include "SystemClock.h"

#define MAX_MARIO_JUMP_SUSTAIN 66


void UnitTest3::CreateMario() {
	uint mario_max_speed_x = main_config.GetUint("M_MAX_SPEED_X");
	uint mario_acceleration_x = main_config.GetUint("M_ACCELERATION_X");
	uint mario_air_acceleration_x = main_config.GetUint("M_AIR_ACCELERATION_X");
	
	int mario_acceleration_y = - main_config.GetInt("M_ACCELERATION_Y");
	int mario_initial_jump_speed = - main_config.GetInt("M_INIT_JUMP_SPEED");

	auto mario_pos = map_info_parser.GetPoint("MARIO");
	auto mario_film = AnimationFilmHolder::Get().GetFilm("MARIO");
	auto* mario_start_animation = (FrameListAnimation*)AnimationHolder::Get().GetAnimation("MARIO_SR");
	mario = new Sprite(mario_pos.x, mario_pos.y, mario_film, "MARIO");
	mario->SetVelocity({0,0});
	auto* mario_animator = new FrameListAnimator();
	mario->main_animator = (MovingAnimator*)mario_animator;
	mario_animator->Start(mario_start_animation, SystemClock::Get().milli_secs());

	auto* mario_tick_animation = (TickAnimation*)AnimationHolder::Get().GetAnimation("MARIO_MOVE");
	auto* mario_tick_animator = new TickAnimator();

	double delay = (float)mario_tick_animation->GetDelay() / 1000;

	// TODO MOTION:
	// MARIO SMTIMES FALLIN FASTER WTF
	// SPRINT LAT3R? 
	mario_tick_animator->SetOnAction(
		[&, delay, mario_max_speed_x, mario_acceleration_x, mario_acceleration_y, mario_initial_jump_speed, mario_air_acceleration_x]
		(Animator* animator, const Animation& anim) {
			auto mario_vel = mario->GetVelocity();
			float x, y, dx, dy;
			mario->GetPos(x, y);
			double new_vel_x = mario_vel.x;
			double new_vel_y = mario_vel.y;

			static int jump_sustained_loops = 0;
			if (movement_keys[ALLEGRO_KEY_W]) {
				if (mario_jump_cd.HasFinished()) {
					if (!mario->GetGravityHandler().IsFalling()) {
						jump_sustained_loops = 0;
						new_vel_y = (double)mario_initial_jump_speed;
					}
					else if (jump_sustained_loops != -1 && jump_sustained_loops < MAX_MARIO_JUMP_SUSTAIN) {
						// keep going up
						jump_sustained_loops++;
						new_vel_y += mario_acceleration_y * delay;
					}
					else
						jump_sustained_loops = -1;
				}
			}
			else {
				jump_sustained_loops = -1;
			}

			if (movement_keys[ALLEGRO_KEY_S]) {
				std::cout << "IMPLEMENT THIS\n";
			}

			auto actual_x_acceleration = mario->GetGravityHandler().IsFalling() ? mario_air_acceleration_x : mario_acceleration_x;
			if (movement_keys[ALLEGRO_KEY_A]) {
				
				if(new_vel_x >= 0) // hard break
					new_vel_x -= 4.0 * actual_x_acceleration * delay;
				else
					new_vel_x -= actual_x_acceleration * delay;
			}
			else if (movement_keys[ALLEGRO_KEY_D]) {
				if (new_vel_x <= 0) // hard break
					new_vel_x += 4.0 * actual_x_acceleration * delay;
				else
					new_vel_x += actual_x_acceleration * delay;
			}
			else {
				// smooth break
				if(new_vel_x > 0)
					new_vel_x -= 2.0 * actual_x_acceleration * delay;
				else if(new_vel_x < 0)
					new_vel_x += 2.0 * actual_x_acceleration * delay;
			}

			if (new_vel_x > mario_max_speed_x)
				new_vel_x = mario_max_speed_x;
			
			if (new_vel_x < -(double)mario_max_speed_x)
				new_vel_x = -(double)mario_max_speed_x;
			

			dx = new_vel_x * delay;
			dy = new_vel_y * delay;
			FilterGridMotion(mario->GetBoxF(), dx, dy);
			if (dx == 0) { // x motion denied
				new_vel_x = 0;
			}
			if (dy == 0) { // y motion denied
				jump_sustained_loops = -1; // stop the jump
				new_vel_y = 0;
			}

			mario->SetVelocity({ new_vel_x, new_vel_y });
			mario->SetPos(x + dx, y + dy);

			mario->SetUpdateBoundAreaPos((dx != 0) || (dy != 0));

			int dist_to_mid = ((x + dx) + 16 - view_win.x) - DIS_WIDTH / 2;
			if (dist_to_mid > 0) { // if x2 of mario is further than the mid of view_win.
				FilterScrollDistance(view_win.x, view_win.w, dist_to_mid, map_dim.w);
				view_win.x += dist_to_mid;
				tile_win_moved = TileAllignedViewBoundCheck();
			}

			mario->GetGravityHandler().Check(mario->GetBoxF());
		}
	);
	SetMarioGravity(mario);
	mario_tick_animator->Start(*mario_tick_animation, SystemClock::Get().milli_secs());
}

void UnitTest3::SetMarioGravity(Sprite* sprite, TickAnimation* fall_update) {
	auto speed_increase = g_acceleration * ((double)fall_update->GetDelay() / 1000);

	auto& gravity = sprite->GetGravityHandler();
	gravity.SetGravity();
	gravity.SetOnStartFalling(
		[sprite, fall_update, speed_increase, &gravity, this]() {
			auto* gravity_animator = new TickAnimator();
			gravity_animator->SetOnFinish(
				[sprite, fall_update, speed_increase, this](Animator* gravity_animator) {
					gravity_animator->SetOnFinish(nullptr); // this is prob unnecessary
					gravity_animator->SetOnAction(
						[sprite, speed_increase, this](Animator* gravity_animator, const Animation& gravity_fall) {
							auto& current_vel = sprite->GetVelocity();
							auto new_vel_y = current_vel.y + speed_increase;
							if (new_vel_y > max_fall_speed)
								new_vel_y = max_fall_speed;
							sprite->SetVelocity({ current_vel.x, new_vel_y });
						}
					);
					((TickAnimator*)gravity_animator)->Start(*fall_update, SystemClock::Get().milli_secs());
				}
			);
			gravity.SetOnStopFalling(
				[sprite, gravity_animator, this]() {
					mario_jump_cd.Start(*(TickAnimation*)AnimationHolder::Get().GetAnimation("JUMP_CD"), SystemClock::Get().milli_secs());
					AnimatorManager::GetSingleton().AddGarbage(gravity_animator);
				}
			);
			gravity_animator->Start(*(TickAnimation*)AnimationHolder::Get().GetAnimation("FALL_DELAY"), SystemClock::Get().milli_secs());
		}
	);
	gravity.SetOnSolidGround(
		[this](const Rect_f& rect) {
			float dx = 0;
			float dy = 1;
			FilterGridMotion(rect, dx, dy);
			if (dy < 1)
				return true;
			return false;
		}
	);
}


void UnitTest3::SetDefaultGravity(Sprite* sprite, TickAnimation* fall_update) {
	auto speed_increase = g_acceleration * ((double)fall_update->GetDelay() / 1000);

	auto& gravity = sprite->GetGravityHandler();
	gravity.SetGravity();
	gravity.SetOnStartFalling(
		[sprite, fall_update, speed_increase, &gravity, this]() {
			auto* gravity_animator = new TickAnimator();
			gravity_animator->SetOnFinish(
				[sprite, fall_update, speed_increase, this](Animator* gravity_animator) {
					gravity_animator->SetOnFinish(nullptr); // this is prob unnecessary
					gravity_animator->SetOnAction(
						[sprite, speed_increase, this](Animator* gravity_animator, const Animation& gravity_fall) {
							auto& current_vel = sprite->GetVelocity();
							auto new_vel_y = current_vel.y + speed_increase;
							if (new_vel_y > max_fall_speed)
								new_vel_y = max_fall_speed;
							sprite->SetVelocity({ current_vel.x, new_vel_y });
						}
					);
					((TickAnimator*)gravity_animator)->Start(*fall_update, SystemClock::Get().milli_secs());
				}
			);
			gravity.SetOnStopFalling(
				[sprite, gravity_animator]() {
					auto current_vel = sprite->main_animator->GetAnim().GetVelocity();
					if(sprite->GetTypeId() != "MARIO") // please fix me
						sprite->SetVelocity({ current_vel.x, 0 });

					AnimatorManager::GetSingleton().AddGarbage(gravity_animator);
				}
			);
			gravity_animator->Start(*(TickAnimation*)AnimationHolder::Get().GetAnimation("FALL_DELAY"), SystemClock::Get().milli_secs());
		}
	);
	gravity.SetOnSolidGround(
		[this](const Rect_f& rect) {
			float dx = 0;
			float dy = 1;
			FilterGridMotion(rect, dx, dy);
			if (dy < 1)
				return true;
			return false;
		}
	);
}


// true = left, false = right
static inline void DefaultOrientationSet(Sprite* sprite, bool orientation) {
	if(!orientation)	// go right
		sprite->main_animator->Start(	(MovingAnimation*)(AnimationHolder::Get().GetAnimation(sprite->GetState() + "R")),
										SystemClock::Get().milli_secs());
	else				// go left
		sprite->main_animator->Start(	(MovingAnimation*)(AnimationHolder::Get().GetAnimation(sprite->GetState() + "L")),
										SystemClock::Get().milli_secs());
}

template <class T1, class T2>
void CreateGoombaAnimators(std::list<Sprite*> sprites, T2* animation) {
	for (auto* sprite : sprites) {
		T1* animator = new T1();
		animator->SetOnStart(
			[sprite](Animator* animator, const Animation& anim) {
				sprite->SetVelocity(anim->GetVelocity());
				//sprite->SetFilm
			}
		);
		animator->SetOnAction(
			[sprite](Animator* animator, const Animation& anim) {
				//sprite->Move( ((const T2&)anim).GetDx(), ((const T2&)anim).GetDy());
				sprite->SetFrame(((T1*)(animator))->GetCurrFrame());
			}
		);
		animator->Start(animation, SystemClock::Get().milli_secs());
	}
}

static void DefaultCollisionHandler(Sprite* s1, Sprite* s2) {
	std::cout << "Collision between:" << s1->GetTypeId() << " and " << s2->GetTypeId() << std::endl;
	auto& s1_vel = s1->GetVelocity();
	auto& s2_vel = s2->GetVelocity();
	// this is not the way
	auto relative_directions = s1_vel.x * s2_vel.x;
	if (relative_directions < 0) { // face 2 face
		DefaultOrientationSet(s1, s1_vel.x > 0);
		DefaultOrientationSet(s2, s2_vel.x > 0);
	}
	else if (relative_directions == 0) {
		if (s1_vel.x == 0)
			DefaultOrientationSet(s2, s2_vel.x > 0);
		else
			DefaultOrientationSet(s1, s1_vel.x > 0);
	}
	else {
		float x1, x2, tmp;
		s1->GetPos(x1, tmp);
		s2->GetPos(x2, tmp);
		if (s1_vel.x > 0) {
			x1 < x2 ? DefaultOrientationSet(s1, true) : DefaultOrientationSet(s2, true);
		}
		else {
			x1 < x2 ? DefaultOrientationSet(s2, false) : DefaultOrientationSet(s1, false);
		}
	}
}

/*
* T1: Animator sub class
* T2: Animation sub class
* 
* For each sprite in list, creates new Animator, 
* add animation to the animator.
*/
template <class T1, class T2>
void CreateStaticAnimators(std::list<Sprite*> sprites, T2* animation) {
	for (auto* sprite : sprites) {
		T1* animator = new T1();
		animator->SetOnAction(
			[sprite](Animator* animator, const Animation& anim) {
				sprite->SetFrame(((T1*)(animator))->GetCurrFrame());
			}
		);
		animator->Start(animation, SystemClock::Get().milli_secs());
	}
}

/*
* T1: Animator sub class
* T2: Animation sub class
*
* For each sprite in list, creates new Animator,
* add animation to the animator.
*/
template <class T1, class T2>
void CreateMovingAnimators(std::list<Sprite*> sprites, T2* animation) {
	for (auto* sprite : sprites) {
		T1* animator = new T1();
		animator->SetOnStart(
			[sprite](Animator* animator, const Animation& anim) {
				auto& anim_vel = ((const T2&)anim).GetVelocity();
				if (sprite->GetGravityHandler().IsFalling()) {
					auto& sprite_vel = sprite->GetVelocity();
					sprite->SetVelocity({ 0, sprite_vel.y });
				}
				else {
					sprite->SetVelocity(anim_vel);
				}
				//TODO: change film
				//sprite->
			}
		);
		animator->SetOnAction(
			[sprite](Animator* animator, const Animation& anim) {
				sprite->SetFrame(((T1*)(animator))->GetCurrFrame());
			}
		);
		animator->Start(animation, SystemClock::Get().milli_secs());
		sprite->SetVelocity(animation->GetVelocity());
		sprite->main_animator = (MovingAnimator*)animator;
	}
}


//std::vector<int> GetListInt(std::string&);
std::list<Sprite*> UnitTest3::LoadSpriteList(std::vector<std::string>& list, const AnimationFilm* sprite_film, const std::string& sprite_type, const std::string& default_state) {
	std::list<Sprite*> sprites;
	Sprite* sprite;
	for (auto& str : list) {
		auto sprite_loc = map_info_parser.GetPoint(str);

		sprite = new Sprite(sprite_loc.x, sprite_loc.y, sprite_film, sprite_type);
		sprite->SetState(default_state);
		sprites.push_front(sprite);
	}
	return sprites;
}

void UnitTest3::SpriteLoader() {
	auto& film_holder = AnimationFilmHolder::Get();
	auto& anim_holder = AnimationHolder::Get();
#if 1
	
	auto npc_type_list = main_config.GetList("NPCS");
	for (auto& npc_type : npc_type_list) {
		auto npc_type_info = main_config.GetList(npc_type); // [0] = type(range/list), [1] = is_static, [2] = init_state, [3] = init_anim
		auto sprite_list_str = map_info_parser.GetList(npc_type);
		auto sprite_film = film_holder.GetFilm(npc_type);
		auto sprites_loaded = LoadSpriteList(sprite_list_str, sprite_film, npc_type, npc_type_info[2]);
		
		if (npc_type_info[0] == "range") { // if sprite is range
			auto* fr_animation = (FrameRangeAnimation*)anim_holder.GetAnimation(npc_type_info[3]);
			if (npc_type_info[1] == "false") { // if sprite is moving:
				moving_sprites.insert(moving_sprites.end(), sprites_loaded.begin(), sprites_loaded.end());
				CreateMovingAnimators<FrameRangeAnimator, FrameRangeAnimation>(sprites_loaded, fr_animation);
			}
			else
				CreateStaticAnimators<FrameRangeAnimator, FrameRangeAnimation>(sprites_loaded, fr_animation);
		}
		else if (npc_type_info[0] == "list") { // if sprite is list
			auto* fl_animation = (FrameListAnimation*)anim_holder.GetAnimation(npc_type_info[3]);
			if (npc_type_info[1] == "false") { // if sprite is moving:
				moving_sprites.insert(moving_sprites.end(), sprites_loaded.begin(), sprites_loaded.end());
				CreateMovingAnimators<FrameListAnimator, FrameListAnimation>(sprites_loaded, fl_animation);
			}
			else
				CreateStaticAnimators<FrameListAnimator, FrameListAnimation>(sprites_loaded, fl_animation);
		}
		else { // if type is not range or list
			assert(0);
		}
	}


#else
	auto sprite_list_str = map_info_parser.GetList(sprite_name);
	auto sprite_film = film_holder.GetFilm(sprite_name);
	auto sprites_loaded = LoadSpriteList(sprite_list_str, sprite_film, sprite_name, "-");									// Get a sprite list of all coins.
	auto* fr_animation = (FrameRangeAnimation*) anim_holder.GetAnimation(sprite_name);								// Same animation ptr for all coin animators.
	CreateStaticAnimators<FrameRangeAnimator, FrameRangeAnimation>(sprites_loaded, fr_animation);					// Create Animators for all coins.

	sprite_list_str = map_info_parser.GetList(sprite_name);
	sprite_film = film_holder.GetFilm(sprite_name);
	sprites_loaded = LoadSpriteList(sprite_list_str, sprite_film, sprite_name, "-");
	fr_animation = (FrameRangeAnimation*)anim_holder.GetAnimation(sprite_name);
	CreateStaticAnimators<FrameRangeAnimator, FrameRangeAnimation>(sprites_loaded, fr_animation);

	sprite_list_str = map_info_parser.GetList(sprite_name);
	sprite_film = film_holder.GetFilm(sprite_name);
	sprites_loaded = LoadSpriteList(sprite_list_str, sprite_film, sprite_name, "GOOMBA_W");
	moving_sprites.insert(moving_sprites.end(), sprites_loaded.begin(), sprites_loaded.end());
	fr_animation = (FrameRangeAnimation*)anim_holder.GetAnimation(sprite_name);
	CreateMovingAnimators<FrameRangeAnimator, FrameRangeAnimation>(sprites_loaded, fr_animation);

	sprite_list_str = map_info_parser.GetList("GKT");
	sprite_film = film_holder.GetFilm("gkt");
	sprites_loaded = LoadSpriteList(sprite_list_str, sprite_film, "GKT", "GKT_W");
	moving_sprites.insert(moving_sprites.end(), sprites_loaded.begin(), sprites_loaded.end());
	auto fl_animation = (FrameListAnimation*)anim_holder.GetAnimation("GKT_WR");
	CreateMovingAnimators<FrameListAnimator, FrameListAnimation>(sprites_loaded, fl_animation);

#endif
	for (auto* sprite : moving_sprites) {
		for (auto* other_sprite : moving_sprites) {
			if (sprite != other_sprite) {
				collision_checker.Register(sprite, other_sprite, DefaultCollisionHandler);
			}
		}
	}

	for (auto* sprite : moving_sprites)
		SetDefaultGravity(sprite);

	//for (auto* sprite : moving_sprites) { // assume that moving sprites have default fall anim
	//	auto& gravity = sprite->GetGravityHandler();
	//	gravity.SetGravity();
	//	gravity.SetOnStartFalling(
	//		[sprite, &anim_holder, &gravity]() {
	//			auto* gravity_animator = new TickAnimator();
	//			gravity_animator->SetOnFinish(
	//				[sprite, &anim_holder](Animator* gravity_animator) {
	//					gravity_animator->SetOnFinish(nullptr); // this is prob unnecessary
	//					gravity_animator->SetOnAction(
	//						[sprite](Animator* gravity_animator, const Animation& gravity_fall) {
	//							auto& current_vel = sprite->GetVelocity();
	//							sprite->SetVelocity({ current_vel.x, current_vel.y + G_ACCELERATION });
	//						}
	//					);
	//					((TickAnimator*)gravity_animator)->Start(*(TickAnimation*)anim_holder.GetAnimation("FALL_UPDATE"), SystemClock::Get().milli_secs());
	//				}
	//			);
	//			gravity.SetOnStopFalling(
	//				[sprite, gravity_animator]() {
	//					auto current_vel = sprite->main_animator->GetAnim().GetVelocity();
	//					sprite->SetVelocity({ current_vel.x, 0 });
	//					AnimatorManager::GetSingleton().AddGarbage(gravity_animator);
	//				}
	//			);
	//			gravity_animator->Start(*(TickAnimation*)anim_holder.GetAnimation("FALL_DELAY"), SystemClock::Get().milli_secs());
	//		}
	//	);
	//	gravity.SetOnSolidGround(
	//		[this](const Rect_f& rect) {
	//			float dx = 0;
	//			float dy = 1;
	//			FilterGridMotion(rect, dx, dy);
	//			if (dy < 1)
	//				return true;
	//			return false;
	//		}
	//	);
	//}
}

UnitTest3::UnitTest3() : 
			sprite_manager(SpriteManager::GetSingleton()), 
			animator_manager(AnimatorManager::GetSingleton()), 
			collision_checker(CollisionChecker::GetSingleton()) {
	main_config.SetNewParser("UnitTests/UnitTest3/media/main_config.data");
	max_fall_speed = main_config.GetDouble("MAX_FALL_SPEED");
	g_acceleration = main_config.GetDouble("G_ACCELERATION");

	input_mario = [&] {
		if (kb_event_b) {
			if (kb_event.type == ALLEGRO_EVENT_KEY_DOWN) {
				movement_keys[kb_event.keyboard.keycode] = true;
			}
			else if (kb_event.type == ALLEGRO_EVENT_KEY_UP) {
				movement_keys[kb_event.keyboard.keycode] = false;
			}
		}
	};
	
	display_texts = [&] {
		// ----- static -----
		al_draw_text(font0, font0_color, 96, 5, ALLEGRO_ALIGN_CENTRE, "SCORE");
		al_draw_text(font0, font0_color, 288, 5, ALLEGRO_ALIGN_CENTRE, "COINS");
		al_draw_text(font0, font0_color, 480, 5, ALLEGRO_ALIGN_CENTRE, "WORLD");
		al_draw_text(font0, font0_color, 672, 5, ALLEGRO_ALIGN_CENTRE, "TIME");
		al_draw_text(font0, font0_color, 864, 5, ALLEGRO_ALIGN_CENTRE, "LIVES");
		// ----- non-static -----
		al_draw_textf(font0, font0_color, 96, 32, ALLEGRO_ALIGN_CENTRE, "%u", score);
		al_draw_textf(font0, font0_color, 288, 32, ALLEGRO_ALIGN_CENTRE, "%u", coins);
		al_draw_textf(font0, font0_color, 480, 32, ALLEGRO_ALIGN_CENTRE, "%s", map_id.c_str());
		al_draw_textf(font0, font0_color, 672, 32, ALLEGRO_ALIGN_CENTRE, "%u", time_left);
		al_draw_textf(font0, font0_color, 864, 32, ALLEGRO_ALIGN_CENTRE, "%u", lives);
	};

	animator_refresh = [&] {
		animator_manager.Progress(SystemClock::Get().milli_secs());
	};

	display_sprites = [&] {
		auto& display_list = sprite_manager.GetDisplayList();
		for(auto& i: display_list){
			i->Display(NULL, view_win, default_clipper);
		}
	};
	
	
	rect_movement = [&] {
		ALLEGRO_EVENT timer_event;
		if (al_get_next_event(timer_queue0, &timer_event)) {
			rect_mvmnt.y = 0;
			rect_mvmnt.x = 0;
			if (movement_keys[ALLEGRO_KEY_W]) {
				rect_mvmnt.y -= RECT_MAX_SPEED_Y;
			}
			if (movement_keys[ALLEGRO_KEY_S]) {
				rect_mvmnt.y += RECT_MAX_SPEED_Y;
			}
			if (movement_keys[ALLEGRO_KEY_A]) {
				rect_mvmnt.x -= RECT_MAX_SPEED_X;
			}
			if (movement_keys[ALLEGRO_KEY_D]) {
				rect_mvmnt.x += RECT_MAX_SPEED_X;
			}

			float dx, dy;
			dx = rect_mvmnt.x * RECT_UPDATE_POS;
			dy = rect_mvmnt.y * RECT_UPDATE_POS;
			FilterGridMotion(rectangle, dx, dy);
			rectangle.x += dx;
			rectangle.y += dy;
			if (rectangle.x < view_win.x) // cannot go left
				rectangle.x = view_win.x;
			// this below isnt real distance. its x coord compared to mid :O
			int dist_to_mid = (rectangle.x + rectangle.w - view_win.x) - DIS_WIDTH / 2;
			if (dist_to_mid > 0) { // if x2 of mario is further than the mid of view_win.
				FilterScrollDistance(view_win.x, view_win.w, dist_to_mid, map_dim.w);
				view_win.x += dist_to_mid;
				tile_win_moved = TileAllignedViewBoundCheck();
			}
		}
	};
}


void UnitTest3::Initialise(void) {
	UnitTest2::Initialise();
	al_init_font_addon();
	al_init_ttf_addon();
	Clipper::InitViewWindow(&view_win);


	AnimationFilmHolder::Get().LoadAll(FILMS_DATA_PATH, FilmParser);
	AnimationHolder::Get().LoadAll(	ANIMS_FRAME_LIST_PATH,
									ANIMS_FRAME_RANGE_PATH,
									ANIMS_TICK_PATH );

	auto font0_path = main_config.GetStr("FONT0");
	auto font0_size = main_config.GetInt("FONT0_SIZE");
	font0 = al_load_font(font0_path.c_str(), font0_size, 0);
	font0_color = al_map_rgb(255, 255, 255);

	map_id = map_info_parser.GetStr("MAP_ID");
	time_left = map_info_parser.GetUint("TIME");
	lives = map_info_parser.GetUint("LIVES");
	CreateMario();
}


void UnitTest3::Load(void) {
	//game.PushbackCollisions(collision_check); // this is not the way.
	game.PushbackRender(render_terrain);
	game.PushbackRender(render_rect);
	game.PushbackRender(display_sprites);
	game.PushbackRender(display_texts);
	game.PushbackRender(flip_display);
	game.PushbackInput(input_events0);
	//game.PushbackInput(input_rect);
	game.PushbackInput(input_mario);
	//game.PushbackInput(input_scroll);
	//game.PushbackPhysics(physics_rect);
	//game.PushbackPhysics(rect_movement);
	game.PushbackAnim(animator_refresh);
	SpriteLoader();

	// ------------------------------------
	auto* default_mover_refresh = (TickAnimation*)AnimationHolder::Get().GetAnimation("DEFAULT_MOVER");
	float time_delay = (float)default_mover_refresh->GetDelay() / 1000;
	std::cout << time_delay << std::endl;
		//new TickAnimation("timer", 16, 0, true);
	auto* mover_animator = new TickAnimator();
	mover_animator->SetOnAction(
		[&, time_delay](Animator* animator, const Animation& anim) {
			for (auto* sprite : moving_sprites) {
				float dx, dy, x, y;
				sprite->GetPos(x, y);
				auto velocity = sprite->GetVelocity();
				dx = velocity.x * time_delay;
				dy = velocity.y * time_delay;
				FilterGridMotion(sprite->GetBoxF(), dx, dy);
				sprite->SetPos(x + dx, y + dy);
				// remove this.
				if ((velocity.x != 0) && (dx == 0)) { // motion denied
					DefaultOrientationSet(sprite, velocity.x > 0);
				}
				sprite->SetUpdateBoundAreaPos((dx != 0) || (dy != 0));
				//sprite->SetVelocity({ -sprite->GetVelocity().x, 0 });
				//if (dx == 0) // motion denied
					//sprite->main_animator->Start(goleft/goright, TIME);
				sprite->GetGravityHandler().Check(sprite->GetBoxF());
			}
			collision_checker.Check();
		}
	);
	mover_animator->Start(*default_mover_refresh, SystemClock::Get().milli_secs());
	// ------------------------------------

	auto* every_second_tick = (TickAnimation*)AnimationHolder::Get().GetAnimation("EVRY_SEC");
	auto* every_second_animator = new TickAnimator();
	every_second_animator->SetOnAction(
		[&](Animator* animator, const Animation& anim) {
			time_left--;
			if (time_left == 0) {
				time_is_up = true;
				std::cout << "YOU NOOB\n";
			}
		}
	);
	every_second_animator->Start(*every_second_tick, SystemClock::Get().milli_secs());
}

void UnitTest3::Clear(void) {
	al_destroy_font(font0);
	UnitTest2::Clear();
	auto b = AnimationFilmHolder::Get().GetBitmapLoader();
	b.CleanUp(); //clean bitmaps
	AnimationFilmHolder::Get().CleanUp(); //clean animation films
}

void UnitTest3::Main() {
	UnitTest2::Main();
}
#include "Level3.h"

#include "Util.h"
#include "SystemClock.h"

static inline void DefaultOrientationChange(Sprite* sprite, bool orientation) {
	if(!orientation) // go right
		sprite->main_animator->Start(	(MovingAnimation*)(AnimationHolder::Get().GetAnimation(sprite->GetState() + "R")),
										SystemClock::Get().milli_secs());
	else // go left
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
				sprite->SetVelocity(((const T2&)anim).GetVelocity());
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

	auto sprite_list_str = map_info_parser.GetList("COIN");
	auto sprite_film = film_holder.GetFilm("coin");
	auto sprites_loaded = LoadSpriteList(sprite_list_str, sprite_film, "COIN", "-");									// Get a sprite list of all coins.
	auto* fr_animation = (FrameRangeAnimation*) anim_holder.GetAnimation("COIN");								// Same animation ptr for all coin animators.
	CreateStaticAnimators<FrameRangeAnimator, FrameRangeAnimation>(sprites_loaded, fr_animation);					// Create Animators for all coins.

	sprite_list_str = map_info_parser.GetList("Q_MARK");
	sprite_film = film_holder.GetFilm("q_mark");
	sprites_loaded = LoadSpriteList(sprite_list_str, sprite_film, "QMARK", "-");
	fr_animation = (FrameRangeAnimation*)anim_holder.GetAnimation("QMARK");
	CreateStaticAnimators<FrameRangeAnimator, FrameRangeAnimation>(sprites_loaded, fr_animation);

	sprite_list_str = map_info_parser.GetList("GOOMBA");
	sprite_film = film_holder.GetFilm("goomba");
	sprites_loaded = LoadSpriteList(sprite_list_str, sprite_film, "GOOMBA", "GOOMBA_W");
	moving_sprites.insert(moving_sprites.end(), sprites_loaded.begin(), sprites_loaded.end());
	fr_animation = (FrameRangeAnimation*)anim_holder.GetAnimation("GOOMBA_WR");
	CreateMovingAnimators<FrameRangeAnimator, FrameRangeAnimation>(sprites_loaded, fr_animation);

	sprite_list_str = map_info_parser.GetList("GKT");
	sprite_film = film_holder.GetFilm("gkt");
	sprites_loaded = LoadSpriteList(sprite_list_str, sprite_film, "GKT", "GKT_W");
	moving_sprites.insert(moving_sprites.end(), sprites_loaded.begin(), sprites_loaded.end());
	auto fl_animation = (FrameListAnimation*)anim_holder.GetAnimation("GKT_WR");
	CreateMovingAnimators<FrameListAnimator, FrameListAnimation>(sprites_loaded, fl_animation);
	
	//auto coin_animation = new FrameRangeAnimation("coin", 0, sprite_film->GetTotalFrames(), 0, 0, 0, 500);
	//auto a_coin_animator = new FrameRangeAnimator();
	//tmp(sprite_film);

	//sprite_list = map_info_parser.GetList("QMARK");
	//sprite_film = film_holder.GetFilm("\"qid\"");  // <-- change this.
	//LoadSpriteList(sprite_list, sprite_film, "q_mark");

	// ....
}

UnitTest3::UnitTest3() : sprite_manager(SpriteManager::GetSingleton()), animator_manager(AnimatorManager::GetSingleton()) {

	animator_refresh = [&] {
		animator_manager.Progress(SystemClock::Get().milli_secs());
	};

	display_sprites = [&] {
		auto& display_list = sprite_manager.GetDisplayList();
		for(auto& i: display_list){
			i->Display(NULL, view_win, default_clipper);
		}
	};
	
	
	mario_physics = [&] {
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
	Clipper::InitViewWindow(&view_win);


	AnimationFilmHolder::Get().LoadAll(FILMS_DATA_PATH, FilmParser);
	AnimationHolder::Get().LoadAll(	ANIMS_FRAME_LIST_PATH,
									ANIMS_FRAME_RANGE_PATH,
									ANIMS_FRAME_TICK_PATH );
}


void UnitTest3::Load(void) {
	game.PushbackRender(render_terrain);
	game.PushbackRender(render_rect);
	game.PushbackRender(display_sprites);
	game.PushbackRender(flip_display);
	game.PushbackInput(input_events0);
	game.PushbackInput(input_rect);
	//game.PushbackInput(input_scroll);
	//game.PushbackPhysics(physics_rect);
	game.PushbackPhysics(mario_physics);
	game.PushbackAnim(animator_refresh);
	UnitTest3::SpriteLoader();
	TickAnimation* refresh_60hz = new TickAnimation("timer", 16, 0, true);
	auto* mover_animator = new TickAnimator();
	mover_animator->SetOnAction(
		[&](Animator* animator, const Animation& anim) {
			for (auto* sprite : moving_sprites) {
				float dx, dy, x, y;
				sprite->GetPos(x, y);
				auto velocity = sprite->GetVelocity();
				dx = velocity.x * 0.016;
				dy = velocity.y * 0.016;
				FilterGridMotion(sprite->GetBoxF(), dx, dy);
				sprite->SetPos(x + dx, y + dy);
				// remove this.
				if ((velocity.x != 0) && (dx == 0.0f)) { // motion denied
					DefaultOrientationChange(sprite, velocity.x > 0);
				}
					
				//sprite->SetVelocity({ -sprite->GetVelocity().x, 0 });
				//if (dx == 0) // motion denied
					//sprite->main_animator->Start(goleft/goright, TIME);
			}
		}
	);
	mover_animator->Start(*refresh_60hz, SystemClock::Get().milli_secs());
}

void UnitTest3::Clear(void) {
	UnitTest2::Clear();
	auto b = AnimationFilmHolder::Get().GetBitmapLoader();
	b.CleanUp(); //clean bitmaps
	AnimationFilmHolder::Get().CleanUp(); //clean animation films
}

void UnitTest3::Main() {
	UnitTest2::Main();
}
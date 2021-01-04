#include "Level3.h"

#include "Util.h"
#include "Sprite.h"


void UnitTest3::LoadSpriteList(std::vector<std::string>& list, const AnimationFilm* sprite_film, const std::string& sprite_type) {
	Sprite* created_sprite;
	for (auto& str : list) {
		auto sprite_loc = map_info_parser.GetPoint(str);
		created_sprite = new Sprite(sprite_loc.x, sprite_loc.y, sprite_film, sprite_type);
		// do more here? Set Mover etc.
	}
}

void UnitTest3::SpriteLoader() {
	auto& film_holder = AnimationFilmHolder::Get();

	auto sprite_list = map_info_parser.GetList("COIN");
	auto sprite_film = film_holder.GetFilm("coin");
	LoadSpriteList(sprite_list, sprite_film, "coin");

	//sprite_list = map_info_parser.GetList("QMARK");
	//sprite_film = film_holder.GetFilm("\"qid\"");  // <-- change this.
	//LoadSpriteList(sprite_list, sprite_film, "q_mark");

	// ....

}

UnitTest3::UnitTest3() : sprite_manager(SpriteManager::GetSingleton()) {

	display_sprites = [&] {
		auto& display_list = sprite_manager.GetDisplayList();
		for(auto& i: display_list){
			i->Display(NULL, view_win, default_clipper);
		}
	};
	
	
	mario_physics = [&] {
		ALLEGRO_EVENT timer_event;
		if (al_get_next_event(timer_queue0, &timer_event)) {
			rect_mvmnt.y_speed = 0;
			rect_mvmnt.x_speed = 0;
			if (movement_keys[ALLEGRO_KEY_W]) {
				rect_mvmnt.y_speed -= RECT_MAX_SPEED_Y;
			}
			if (movement_keys[ALLEGRO_KEY_S]) {
				rect_mvmnt.y_speed += RECT_MAX_SPEED_Y;
			}
			if (movement_keys[ALLEGRO_KEY_A]) {
				rect_mvmnt.x_speed -= RECT_MAX_SPEED_X;
			}
			if (movement_keys[ALLEGRO_KEY_D]) {
				rect_mvmnt.x_speed += RECT_MAX_SPEED_X;
			}

			float dx, dy;
			dx = rect_mvmnt.x_speed * RECT_UPDATE_POS;
			dy = rect_mvmnt.y_speed * RECT_UPDATE_POS;
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
#if 1
#define FILMS_DATA_PATH "UnitTests/UnitTest3/media/films.data"

	AnimationFilmHolder::Get().LoadAll(FILMS_DATA_PATH, FilmParser);

#endif
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

	UnitTest3::SpriteLoader();
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
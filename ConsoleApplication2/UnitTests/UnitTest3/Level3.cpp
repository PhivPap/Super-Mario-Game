#include "Level3.h"


UnitTest3::UnitTest3(){
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

	
	// DUMMY CODE
	/*auto animations = map_info_parser.GetList("ANIMATIONS");
	ConfigParser coin_parser;
	ConfigParser q_mark_parser;
	coin_parser.SetNewParser(map_info_parser.GetStr(animations[0]).c_str());
	q_mark_parser.SetNewParser(map_info_parser.GetStr(animations[1]).c_str());
	std::cout << coin_parser.GetStr("BMP_PATH") << std::endl;
	std::cout << q_mark_parser.GetStr("BMP_PATH") << std::endl;*/
	// test 1 -2  commit
}

#include "Animation/AnimationFilmHolder.h"

void hello(const char* s) { std::cout << s << std::endl; }

void test(const std::function<void(const char* s)>& loader) {
	loader("cpp is lit");
}

void UnitTest3::Initialise(void) {
	UnitTest2::Initialise();

/* Demo program of parsing a .data file with films information*/
#if 0
#define FILMS_DATA_PATH "UnitTests/UnitTest3/media/films.data"
	
	// struct test represents animation film data
	struct test { 
		std::string id;
		std::string path;
		std::vector<Rect> rects;
	};

	std::list<struct test> list; // list to hold animation films data
	ConfigParser film_parser;
	film_parser.SetNewParser(FILMS_DATA_PATH);

	for (auto& i : film_parser.GetList("FILMS")) { // for each film
		struct test tmp;
		tmp.id = film_parser.GetStr(film_parser.GetList(i)[0]);
		tmp.path = film_parser.GetStr(film_parser.GetList(i)[1]);		
		auto s = film_parser.GetList(film_parser.GetList(i)[2]); 
		
		for (auto& j : s) { // iterate through rects
			tmp.rects.push_back(film_parser.GetRect(j)); // get rekt xd
		}

		list.push_back(tmp);
	}

	// validation
	for (auto i : list) {
		std::cout << "id: " << i.id << ", path: " << i.path << "\nrects: ";
		for (auto j : i.rects)
			std::cout << "[" << j.x << "," << j.y << "," << j.w << "," << j.h << "], ";
		std::cout << std::endl << std::endl;
	}
#endif

}

void UnitTest3::Load(void) {
	game.PushbackRender(render_terrain);
	game.PushbackRender(render_rect);
	game.PushbackRender(flip_display);
	game.PushbackInput(input_events0);
	game.PushbackInput(input_rect);
	//game.PushbackInput(input_scroll);
	//game.PushbackPhysics(physics_rect);
	game.PushbackPhysics(mario_physics);
}

void UnitTest3::Clear(void) {
	UnitTest2::Clear();
}

void UnitTest3::Main() {
	UnitTest2::Main();
}
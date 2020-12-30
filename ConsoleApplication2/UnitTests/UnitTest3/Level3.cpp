#include "Level3.h"
#include "MapInfoParser.h"

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

	MapInfoParser mip("UnitTests/UnitTest3/media/Map0_info.txt");
	Rect rect = mip.GetRect("rect_99");
	Point point = mip.GetPoint("point_99");
	double dbl = mip.GetDouble("double_99");
	int in = mip.GetInt("int_99");
	uint uin = mip.GetUint("uint_99");
	std::string str = mip.GetStr("str_99");

	std::cout << "rect.x: " << rect.x << std::endl;
	std::cout << "rect.y: " << rect.y << std::endl;
	std::cout << "rect.h: " << rect.h << std::endl;
	std::cout << "rect.w: " << rect.w << std::endl;

	std::cout << "point.x: " << point.x << std::endl;
	std::cout << "point.y: " << point.y << std::endl;


	std::cout << "dbl: " << dbl << std::endl;
	std::cout << "in: " << in << std::endl;
	std::cout << "uin: " << uin << std::endl;
	std::cout << "str: " << str << std::endl;



}

void UnitTest3::Initialise(void) {
	UnitTest2::Initialise();
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
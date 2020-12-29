#include "Level3.h"


UnitTest3::UnitTest3(){
	ALLEGRO_EVENT timer_event;
	mario_physics = [&] {
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

			int dx, dy;
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
#include "Level2.h"

//#define GRID_PATH "UnitTests//UnitTest2//Grid//grid1.csv"
//#define GRID_PATH "UnitTests//UnitTest2//Grid//grid2.csv"





void UnitTest2::ReadTextGrid(std::vector<std::vector<byte>>& grid, Dim& grid_dim){
	if (UnitTest::ReadCSV(grid, map_info_parser.GetStr("GRID_PATH").c_str()) == 1) {
		std::cerr << "Could not open grid file\n";
		exit(1);
	}
}


UnitTest2::UnitTest2() {
	render_rect = [=] {
		int rect_x1 = rectangle.x - view_win.x;
		int rect_x2 = rectangle.x + rectangle.w - view_win.x;
		int rect_y1 = rectangle.y - view_win.y;
		int rect_y2 = rectangle.y + rectangle.h - view_win.y;
		if (rect_x1 < 0 || rect_y1 < 0)
			return;
		if (rect_x2 > view_win.x + view_win.w || rect_y2 > view_win.y + view_win.h)
			return;

		if (rect_filled)
			al_draw_filled_rectangle(rect_x1, rect_y1, rect_x2, rect_y2, color);
		else
			al_draw_rectangle(rect_x1, rect_y1, rect_x2, rect_y2, color, 0);
	};

	input_rect = [&] {
		if (kb_event_b) {
			if (kb_event.type == ALLEGRO_EVENT_KEY_DOWN) {
				movement_keys[kb_event.keyboard.keycode] = true;
				if (kb_event.keyboard.keycode == ALLEGRO_KEY_V)
					rect_filled = !rect_filled;
			}
			else if (kb_event.type == ALLEGRO_EVENT_KEY_UP) {
				movement_keys[kb_event.keyboard.keycode] = false;
			}
		}
	};

	physics_rect = [&]() {
		
		// TEMPORARY CODE 
		//update speed 
		//update location based on speed (rect_mvmnt) with timer event
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
		}
	};

}

void UnitTest2::Initialise(void) {
	UnitTest::Initialise();
	al_init_primitives_addon();
	color = al_map_rgb(255, 0, 0);
	timer_queue0 = al_create_event_queue();
	rect_pos_timer = al_create_timer(RECT_UPDATE_POS);
	al_register_event_source(timer_queue0, al_get_timer_event_source(rect_pos_timer));
	al_start_timer(rect_pos_timer);

	Dim d{0};
	ReadTextGrid(grid, d);
}

void UnitTest2::Load(void) {
	game.PushbackRender(render_terrain);
	game.PushbackRender(render_rect);
	game.PushbackRender(flip_display);
	game.PushbackInput(input_events0);
	game.PushbackInput(input_rect);
	game.PushbackInput(input_scroll);
	game.PushbackPhysics(physics_rect);
}

void UnitTest2::Clear(void) {
	UnitTest::Clear();
	al_destroy_timer(rect_pos_timer);
}

void UnitTest2::Main() {
	UnitTest::Main();
}

void UnitTest2::FilterGridMotion(const Rect_f& r, float& dx, float& dy){
	if (dx < 0)
		FilterGridMotionLeft(r, dx);
	else if (dx > 0)
		FilterGridMotionRight(r, dx);

	if (dy < 0)
		FilterGridMotionUp(r, dy);
	else if (dy > 0)
		FilterGridMotionDown(r, dy);
}

void UnitTest2::FilterGridMotionLeft(const Rect_f& r, float& dx){
	float x1_next = r.x + dx;
	if (x1_next < 0)
		dx = -r.x;
	else {
		uint new_col = x1_next / GRID_ELEMENT_WIDTH;
		uint curr_col = r.x / GRID_ELEMENT_WIDTH;
		if (new_col != curr_col) {
			uint top_row = r.y / GRID_ELEMENT_HEIGHT;
			uint bottom_row = (r.y + r.h - 1) / GRID_ELEMENT_HEIGHT;
			for (uint row = top_row; row <= bottom_row; row++) {
				if (!CanPassGridTile(row, new_col, GRID_SOLID_TILE)) {
					dx = curr_col * GRID_ELEMENT_WIDTH - r.x;
					break;
				}
			}
		}
	}
}

void UnitTest2::FilterGridMotionRight(const Rect_f& r, float& dx){
	float x2 = r.x + r.w - 1;
	float x2_next = x2 + dx;
	if (x2_next >= (int)map_dim.w)
		dx = map_dim.w - x2 - 1;
	else {
		uint new_col = x2_next / GRID_ELEMENT_WIDTH;
		uint curr_col = x2 / GRID_ELEMENT_WIDTH;
		if (new_col != curr_col) {
			uint top_row = r.y / GRID_ELEMENT_HEIGHT;
			uint bottom_row = (r.y + r.h - 1) / GRID_ELEMENT_HEIGHT;
			for (uint row = top_row; row <= bottom_row; row++) {
				if (!CanPassGridTile(row, new_col, GRID_SOLID_TILE)) {
					dx = new_col * GRID_ELEMENT_WIDTH - x2 - 1;
					break;
				}
			}
		}
	}
}

void UnitTest2::FilterGridMotionUp(const Rect_f& r, float& dy){
	float y1_next = r.y + dy;
	if (y1_next < 0)
		dy = -r.y;
	else {
		uint new_row = y1_next / GRID_ELEMENT_WIDTH;
		uint curr_row = r.y / GRID_ELEMENT_WIDTH;
		if (new_row != curr_row) {
			uint left_col = r.x / GRID_ELEMENT_HEIGHT;
			uint right_col = (r.x + r.w - 1) / GRID_ELEMENT_HEIGHT;
			for (uint col = left_col; col <= right_col; col++) {
				if (!CanPassGridTile(new_row, col, GRID_SOLID_TILE)) {
					dy = curr_row * GRID_ELEMENT_HEIGHT - r.y;
					break;
				}
			}
		}
	}
}

void UnitTest2::FilterGridMotionDown(const Rect_f& r, float& dy){
	float y2 = r.y + r.h - 1;
	float y2_next = y2 + dy;
	if (y2_next >= (int)map_dim.h)
		dy = map_dim.h - y2 - 1;
	else {
		uint new_row = y2_next / GRID_ELEMENT_HEIGHT;
		uint curr_row = y2 / GRID_ELEMENT_HEIGHT;
		if (new_row != curr_row) {
			uint left_col = r.x / GRID_ELEMENT_WIDTH;
			uint right_col = (r.x + r.w - 1) / GRID_ELEMENT_WIDTH;
			for (uint col = left_col; col <= right_col; col++) {
				if (!CanPassGridTile(new_row, col, GRID_SOLID_TILE)) {
					dy = new_row * GRID_ELEMENT_HEIGHT - y2 - 1;
					break;
				}
			}
		}
	}
}

bool UnitTest2::CanPassGridTile(uint row, uint col, byte flags){
	return grid[row][col] == GRID_EMPTY_TILE;
}
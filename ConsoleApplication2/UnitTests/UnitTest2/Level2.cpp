#include "Level2.h"

#define GRID_PATH "UnitTests//UnitTest2//Grid//grid1.csv"
//#define GRID_PATH "UnitTests//UnitTest2//Grid//grid2.csv"
 
 #define RECT_MOVE_DIST 16 // REMOVE?? in pixels

#define GRID_ELEMENT_WIDTH 16 // in pixels
#define GRID_ELEMENT_HEIGHT 16 // in pixels

#if TILE_WIDTH % GRID_ELEMENT_WIDTH != 0
#error "TILE_WIDTH % GRID_ELEMENT_WIDTH must be zero!"
#endif

#if TILE_HEIGHT % GRID_ELEMENT_HEIGHT != 0
#error "TILE_HEIGHT % GRID_ELEMENT_HEIGHT must be zero!"
#endif

#define GRID_BLOCK_COLUMNS		(TILE_WIDTH / GRID_ELEMENT_WIDTH)
#define GRID_BLOCK_ROWS			(TILE_HEIGHT / GRID_ELEMENT_HEIGHT)
#define GRID_ELEMENTS_PER_TILE	(GRID_BLOCK_ROWS * GRID_BLOCK_COLUMNS)
#define GRID_MAX_HEIGHT			(MAX_HEIGHT * GRID_BLOCK_ROWS)
#define GRID_MAX_WIDTH			(MAX_WIDTH * GRID_BLOCK_COLUMNS)

#define GRID_THIN_AIR_MASK		0x0000	// element is ignored
#define GRID_LEFT_SOLID_MASK	0x0001	// bit 0
#define GRID_RIGHT_SOLID_MASK	0x0002	// bit 1
#define GRID_TOP_SOLID_MASK		0x0004	// bit 2
#define GRID_BOTTOM_SOLID_MASK	0x0008	// bit 3
#define GRID_GROUND_MASK		0x0010	// bit 4, keep objects top / bottom (gravity)
#define GRID_FLOATING_MASK		0x0020	// bit 5, keep objects anywhere inside (gravity)

#define GRID_EMPTY_TILE 0
#define GRID_SOLID_TILE 1

#define RECT_MAX_SPEED_X 120 // pixels per second. (p/s)
#define RECT_MAX_SPEED_Y 120  // pixels per second. (p/s)
#define RECT_UPDATE_POS 0.00834 // 120 times per second.

#define GRID_EMPTY_TILE GRID_THIN_AIR_MASK
//#define GRID_SOLID_TILE \
//(GRID_LEFT_SOLID_MASK | GRID_RIGHT_SOLID_MASK | GRID_TOP_SOLID_MASK | GRID_BOTTOM_SOLID_MASK)

void UnitTest2::ReadTextGrid(std::vector<std::vector<byte>>& grid, Dim& grid_dim){
	if (UnitTest::ReadCSV(grid, GRID_PATH) == 1)
		exit(1);

	//grid_dim.w = map_dim.w;
	//grid_dim.h = grid.size() * 
}


UnitTest2::UnitTest2() {
	// no need to call previous constructor.
	// constructors always called: base -> derived.
	// do more here ...
	render_rect = [&] {
		al_draw_filled_rectangle(rectangle.x, rectangle.y, rectangle.x + rectangle.w, rectangle.y + rectangle.h, color);
	};

	input_rect = [&] {
		ALLEGRO_EVENT display_event;


		if (al_get_next_event(display_queue, &display_event)) {
			if (display_event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
				game_finished = true;
		}



		ALLEGRO_EVENT kb_event;
		int dx, dy;
		if(al_get_next_event(keyboard_rect_queue, &kb_event)){
			if (kb_event.type == ALLEGRO_EVENT_KEY_DOWN) {
				movement_keys[kb_event.keyboard.keycode] = true;
			}
			else if (kb_event.type == ALLEGRO_EVENT_KEY_UP) {
				movement_keys[kb_event.keyboard.keycode] = false;
			}


			//if(kb_event.type == ALLEGRO_EVENT_KEY_DOWN){
			//std::cout << "Lvl2: key down\n";
				
				/*if(kb_event.keyboard.keycode == ALLEGRO_KEY_W){
					dx = 0;
					dy = -RECT_MOVE_DIST;
					FilterGridMotion(rectangle, dx, dy);
					rectangle.y += dy;
				}
				else if(kb_event.keyboard.keycode == ALLEGRO_KEY_S){
					dx = 0;
					dy = RECT_MOVE_DIST;
					FilterGridMotion(rectangle, dx, dy);
					rectangle.y += dy;
				}
				else if(kb_event.keyboard.keycode == ALLEGRO_KEY_A){
					dx = -RECT_MOVE_DIST;
					dy = 0;
					FilterGridMotion(rectangle, dx, dy);
					rectangle.x += dx;
				}
				else if(kb_event.keyboard.keycode == ALLEGRO_KEY_D){
					dx = RECT_MOVE_DIST;
					dy = 0;
					FilterGridMotion(rectangle, dx, dy);
					rectangle.x += dx;
				}*/
			//}
		}
	};

	physics_rect = [&]() {
		
		// TEMPORARY CODE 
		//update speed 
		//update location based on speed (rect_mvmnt) with timer event
		ALLEGRO_EVENT timer_event;
		if (al_get_next_event(rect_timer_queue, &timer_event)) {
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
		}
	};

}

void UnitTest2::Initialise(void) {
	UnitTest::Initialise();
	//al_install_system();
	if (!al_init_primitives_addon())
		exit(1);
	color = al_map_rgb(255, 0, 0);
	keyboard_rect_queue = al_create_event_queue();
	rect_timer_queue = al_create_event_queue();
	rect_pos_timer = al_create_timer(RECT_UPDATE_POS);
	al_register_event_source(keyboard_rect_queue, al_get_keyboard_event_source());
	al_register_event_source(rect_timer_queue, al_get_timer_event_source(rect_pos_timer));
	al_start_timer(rect_pos_timer);
	// do more here ...
}

void UnitTest2::Load(void) {
	game.addFirstRender(render_rect);
	game.addFirstInput(input_rect);
	game.addFirstPhysics(physics_rect);
	Dim d{0};
	ReadTextGrid(grid, d);
	UnitTest::Load();
}

void UnitTest2::Clear(void) {
	UnitTest::Clear();
	al_destroy_timer(rect_pos_timer);
	// do more here ...
}

void UnitTest2::Main() {
	UnitTest::Main();
}

void UnitTest2::FilterGridMotion(const Rect& r, int& dx, int& dy){
	if (dx < 0)
		FilterGridMotionLeft(r, dx);
	else if (dx > 0)
		FilterGridMotionRight(r, dx);

	if (dy < 0)
		FilterGridMotionUp(r, dy);
	else if (dy > 0)
		FilterGridMotionDown(r, dy);
}

void UnitTest2::FilterGridMotionLeft(const Rect& r, int& dx){
	int x1_next = r.x + dx;
	if (x1_next < 0)
		dx = -(int)r.x;
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

void UnitTest2::FilterGridMotionRight(const Rect& r, int& dx){
	int x2 = r.x + r.w - 1;
	int x2_next = x2 + dx;
	if (x2_next >= (int)map_dim.w)
		dx = map_dim.w - x2;
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

void UnitTest2::FilterGridMotionUp(const Rect& r, int& dy){
	int y1_next = r.y + dy;
	if (y1_next < 0)
		dy = -(int)r.y;
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

void UnitTest2::FilterGridMotionDown(const Rect& r, int& dy){
	int y2 = r.y + r.h - 1;
	int y2_next = y2 + dy;
	if (y2_next >= (int)map_dim.h)
		dy = map_dim.h - y2;
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
	return grid[row][col] == GRID_EMPTY_TILE; // fix me pls
	//return grid[row][col] & (flags != 0);
}
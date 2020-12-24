#include "Level2.h"

#define RECT_MOVE_DIST 16 // REMOVE?? in pixels

#define GRID_ELEMENT_WIDTH 16 // in pixels
#define GRID_ELEMENT_HEIGHT 16 // in pixels
#define GRID_PATH "UnitTests//UnitTest2//Grid//grid1.csv"
#define GRID_EMPTY_TILE 0
#define GRID_SOLID_TILE 1

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
			//if(kb_event.type == ALLEGRO_EVENT_KEY_DOWN){
			//std::cout << "Lvl2: key down\n";
				if(kb_event.keyboard.keycode == ALLEGRO_KEY_W){
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
				}
			//}
		}
	};
}

void UnitTest2::Initialise(void) {
	UnitTest::Initialise();
	if (!al_init_primitives_addon())
		exit(1);
	color = al_map_rgb(255, 0, 0);
	keyboard_rect_queue = al_create_event_queue();
	al_register_event_source(keyboard_rect_queue, al_get_keyboard_event_source());
	// do more here ...
}

void UnitTest2::Load(void) {
	game.addFirstRender(render_rect);
	UnitTest::Load();
	//game.clearInput();
	game.addFirstInput(input_rect);
	Dim d{0};
	ReadTextGrid(grid, d);
	
	
	// do more here ...
}

void UnitTest2::Clear(void) {
	UnitTest::Clear();
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
	if (x2_next >= map_dim.w)
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
	if (y2_next >= map_dim.h)
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
}
#include "Level2.h"

#define RECT_MOVE_DIST 4 // REMOVE?? in pixels

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
		/*ALLEGRO_EVENT kb_event;
		if(al_get_next_event(keyboard_queue, &kb_event)){
			if(kb_event.type == ALLEGRO_EVENT_KEY_DOWN){
				if(kb_event.keyboard.keycode == ALLEGRO_KEY_W){
					FilterGridMotion();
				}
				else if(kb_event.keyboard.keycode == ALLEGRO_KEY_S){
					FilterGridMotion();
				}
				else if(kb_event.keyboard.keycode == ALLEGRO_KEY_A){
					FilterGridMotion();
				}
				else if(kb_event.keyboard.keycode == ALLEGRO_KEY_D){
					FilterGridMotion();
				}
			}
		}*/
	};
}

void UnitTest2::Initialise(void) {
	UnitTest::Initialise();
	if (!al_init_primitives_addon())
		exit(1);
	color = al_map_rgb(255, 0, 0);
	// do more here ...
}

void UnitTest2::Load(void) {
	game.addFirstRender(render_rect);
	UnitTest::Load();
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
		FilterGridMotionDown(r, dy);
	else if (dy > 0)
		FilterGridMotionUp(r, dy);
}

void UnitTest2::FilterGridMotionLeft(const Rect& r, int& dx){

}

void UnitTest2::FilterGridMotionRight(const Rect& r, int& dx){

}

void UnitTest2::FilterGridMotionUp(const Rect& r, int& dy){

}

void UnitTest2::FilterGridMotionDown(const Rect& r, int& dy){

}

bool UnitTest2::CanPassGridTile(Dim row, Dim col, byte flags){
	//return grid[row][col] == 0;
	return false;
}
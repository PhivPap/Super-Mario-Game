#include "Level2.h"



void UnitTest2::ReadTextGrid(std::vector<std::vector<byte>>& grid, Dim& grid_dim){
	if (UnitTest::ReadCSV(grid, GRIDMAP_PATH) == 1)
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
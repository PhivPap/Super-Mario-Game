#include "Level3.h"

UnitTest3::UnitTest3(){

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
}

void UnitTest3::Clear(void) {
	UnitTest2::Clear();
}

void UnitTest3::Main() {
	UnitTest2::Main();
}
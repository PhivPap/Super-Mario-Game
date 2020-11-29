#include "Engine.h"

void app::App::Run(void) {
	game.MainLoop();
}

void app::App::RunIteration(void){
	game.MainLoopIteration();
}
app::Game& app::App::GetGame(void) {
	return game;
}

const app::Game& app::App::GetGame(void) const {
	return game;
}

void app::App::Main(void) {
	Initialise();
	Load();
	Run();
	Clear();
}
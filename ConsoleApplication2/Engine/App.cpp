#include "Engine.h"
#include <iostream>

namespace app {
	void App::Run(void) {
		game.MainLoop();
	}

	void App::RunIteration(void) {
		game.MainLoopIteration();
	}
	Game& App::GetGame(void) {
		return game;
	}

	const Game& App::GetGame(void) const {
		return game;
	}

	void App::Main(void) {
		Initialise();
		Load();
		Run();
		Clear();
	}
}
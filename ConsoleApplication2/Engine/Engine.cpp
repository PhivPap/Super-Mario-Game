#include "Engine.h"

namespace app {
	
	void Game::Invoke(const Action& f) { if (f) f(); }
	//template <typename Tfunc>
	void Game::SetRender(Action& f) { render = f; }
	void Game::SetInput(Action& f) { input = f; }
	void Game::SetDone(Pred& f) { done = f; }
	
	// rest of setters are similary defined
	void Game::Render(void) { Invoke(render); }
	void Game::ProgressAnimations(void) { Invoke(anim); }
	void Game::Input(void) { Invoke(input); }
	void Game::AI(void) { Invoke(ai); }
	void Game::Physics(void) { Invoke(physics); }
	void Game::CollisionChecking(void) { Invoke(collisions); }
	void Game::CommitDestructions(void) { Invoke(destruct); }
	void Game::UserCode(void) { Invoke(user); }
	bool Game::IsFinished(void) const { return done(); }
	
	void Game::MainLoop(void) {
		while (!IsFinished())
			MainLoopIteration();
	};
	
	void Game::MainLoopIteration(void) {
		Render();
		Input();
		ProgressAnimations();
		AI();
		Physics();
		CollisionChecking();
		CommitDestructions();
		UserCode();
		// FPS calculation ?
	};
};
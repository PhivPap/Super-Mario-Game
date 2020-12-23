#include "Engine.h"

namespace app {
	
	void Game::Invoke(const Action& f_list) { 
		if (!f_list.empty())
			for (auto f : f_list)
				f();
	}
	
	void Game::addFirstRender(std::function<void(void)>& f) { render.push_front(f); }
	void Game::addLastRender(std::function<void(void)>& f) { render.push_back(f); }
	void Game::addFirstInput(std::function<void(void)>& f) { input.push_front(f); }
	void Game::addLastInput(std::function<void(void)>& f) { input.push_back(f); }
	void Game::clearInput() { input.clear(); }
	//template <typename Tfunc>
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
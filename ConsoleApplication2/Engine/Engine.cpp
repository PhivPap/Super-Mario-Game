#include "Engine.h"

namespace app {
	
	void Game::Invoke(const Action& f_list) { 
		if (!f_list.empty())
			for (auto f : f_list)
				f();
	}
	
	void Game::PushbackRender(std::function<void(void)>& f) { render.push_back(f); }
	void Game::PushbackInput(std::function<void(void)>& f) { input.push_back(f); }
	void Game::PushbackPhysics(std::function<void(void)>& f) { physics.push_back(f); }
	void Game::PushbackAnim(std::function<void(void)>& f) { anim.push_back(f); }
	void Game::PushbackDestruct(std::function<void(void)>& f) { destruct.push_back(f); }
	
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
	
	void Game::SetOnPauseResume(std::function<void(void)>& f){
		pause_resume = f;
	}

	void Game::Pause(long long unsigned int t){
		is_paused = true;
		pause_time = t;
		pause_resume();
	}

	void Game::Resume(void){
		is_paused = false;
		pause_resume();
		pause_time = 0;
	}

	bool Game::IsPaused(void) const{
		return is_paused;
	}

	long long unsigned int Game::GetPauseTime(void) const{
		return pause_time;
	}


	void Game::MainLoop(void) {
		while (!IsFinished())
			MainLoopIteration();
	};
	
	void Game::MainLoopIteration(void) {
		Render();
		Input();
		if(!IsPaused()){
			ProgressAnimations();
			AI();
			Physics();
			CollisionChecking();
			CommitDestructions();
			UserCode();
		}
	};
};
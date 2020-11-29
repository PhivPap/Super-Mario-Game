#pragma once
#include <functional>

namespace app {
	class Game { // app::Game namespace, the mother application
	public:
		using Action = std::function<void(void)>;
		using Pred = std::function<bool(void)>;
	private:
		Action render, anim, input, ai, physics, destruct, collisions, user;
		Pred done;
		void Invoke(const Action& f) { if (f) f(); }
	public:
		template <typename Tfunc>
		void SetRender(const Tfunc& f) { render = f; }
		// rest of setters are similary defined
		void Render(void) { Invoke(render); }
		void ProgressAnimations(void) { Invoke(anim); }
		void Input(void) { Invoke(input); }
		void AI(void) { Invoke(ai); }
		void Physics(void) { Invoke(physics); }
		void CollisionChecking(void) { Invoke(collisions); }
		void CommitDestructions(void) { Invoke(destruct); }
		void UserCode(void) { Invoke(user); }
		bool IsFinished(void) const { return !done(); }
		void MainLoop(void) {};
		void MainLoopIteration(void) {};
	};



	class App {
	protected:
		Game game;

	public:
		virtual void	Initialise(void) = 0;
		virtual void	Load(void) = 0;
		virtual void	Run(void);
		virtual void	RunIteration(void);
		Game& GetGame(void);
		const Game& GetGame(void) const;
		virtual void	Clear(void) = 0;
		void Main(void);
	};
};



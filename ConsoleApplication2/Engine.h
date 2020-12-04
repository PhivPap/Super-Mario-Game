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
		void Invoke(const Action& f);
	public:
		//template <typename Tfunc>
		void SetRender(Action& f);
		void SetInput(Action& f);
		void SetDone(Pred& f);
		
		// rest of setters are similary defined
		void Render(void);
		void ProgressAnimations(void);
		void Input(void);
		void AI(void);
		void Physics(void);
		void CollisionChecking(void);
		void CommitDestructions(void);
		void UserCode(void);
		bool IsFinished(void) const;
		void MainLoop(void);
		void MainLoopIteration(void);
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



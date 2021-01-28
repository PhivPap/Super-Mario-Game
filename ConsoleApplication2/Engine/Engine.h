#pragma once
#include <functional>
#include <list>

namespace app {
	class Game { // app::Game namespace, the mother application
	public:
		using Action = std::list<std::function<void(void)>>;
		using Pred = std::function<bool(void)>;

	private:
		Action render, anim, input, ai, physics, destruct, collisions, user;
		std::function<void(void)> pause_resume = [] {};
		Pred done;
		bool is_paused = false;
		long long unsigned int pause_time = 0;
		void Invoke(const Action& );
	
	public:
		void PushbackRender(std::function<void(void)>&);
		void PushbackInput(std::function<void(void)>&);
		void PushbackPhysics(std::function<void(void)>&);
		void PushbackAnim(std::function<void(void)>&);
		void PushbackDestruct(std::function<void(void)>&);

		void SetDone(Pred&);

		void SetOnPauseResume(std::function<void(void)>&);
		void Pause(long long unsigned int);
		void Resume(void);
		bool IsPaused(void) const;
		long long unsigned int GetPauseTime(void) const;
		
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
		Game&			GetGame(void);
		const Game&		GetGame(void) const;
		virtual void	Clear(void) = 0;
		void Main(void);
	};
};



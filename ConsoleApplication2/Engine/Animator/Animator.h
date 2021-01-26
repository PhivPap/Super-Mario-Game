#pragma once
#include "Animation.h"
#include <functional>

typedef uint64_t timestamp_t;

typedef enum class animatorstate_t {
	ANIMATOR_FINISHED	= 0,
	ANIMATOR_RUNNING	= 1,
	ANIMATOR_STOPPED	= 2
} ANIMATORSTATE_T;

class Animator {
public:
	using OnFinish	= std::function<void(Animator*)>;
	using OnStart	= std::function<void(Animator*, const Animation&)>;
	using OnAction	= std::function<void(Animator*, const Animation&)>;

protected:
	timestamp_t		lastTime = 0;
	animatorstate_t state;
	OnFinish		onFinish;
	OnStart			onStart;
	OnAction		onAction;
	void			NotifyStopped(void);
	void			NotifyStarted(const Animation&);
	void			NotifyAction(const Animation&);
	void			Finish(bool isForced = false);

public:
	
	void			Stop(void);
	bool			HasFinished(void) const;
	virtual void	TimeShift(timestamp_t offset);
	virtual void	Progress(timestamp_t currTime) = 0;
	void			SetOnFinish(const OnFinish&);
	void			SetOnStart(const OnStart&);
	void			SetOnAction(const OnAction&);
	//Animator() = default;
	Animator(void);
	Animator(const Animator&) = delete; //??
	Animator(Animator&&) = delete; // here we go agane
	virtual ~Animator();
};
 

#pragma once
#include <map>
#include <list>
#include "Animation.h"
#include <functional>
#include "Types.h"

class AnimationHolder {	
private:
	using Animations = std::map<std::string, Animation*>;
	Animations				animations;
	static AnimationHolder	holder;
	AnimationHolder(void)	= default;
	~AnimationHolder()		= default;
	void					LoadFrameList(const char*);
	void					LoadFrameRange(const char*);
	void					LoadTick(const char*);
	
public:
	static AnimationHolder& Get(void);
	void					LoadAll(const char*, const char*, const char*);
	Animation*				GetAnimation(const std::string&);
};
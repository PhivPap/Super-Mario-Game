#include "AnimationHolder.h"
#include "ConfigParser.h"
#include "Types.h"
#include <assert.h>
#include "FrameRangeAnimation.h"
#include "FrameListAnimation.h"
#include "TickAnimation.h"

AnimationHolder AnimationHolder::holder;
/* FORMAT:
	FrameListAnimation:

		id: fla0, fl1, ...

		fla0: info_0, frame_list_0
		info_0: vel_x, vel_y, reps, delay
		frame_list_0: 0, 1, ...

	FrameRangeAnimation:

		id: fra0, fra1, ...
		fra0: vel_x, vel_y, reps, delay, begin_frame, end_frame
		fra1:	-||-		-||-		-||-		-||-

	TickAnimation:

		id: ta0, ta1, ...
		ta0: reps, delay, discrete
		ta1:	-||-		-||-
*/

void AnimationHolder::LoadFrameList(const char* list_path) {
	assert(list_path);
	ConfigParser fl_parser;
	fl_parser.SetNewParser(list_path);
	auto animations = fl_parser.GetList("id");
	for (auto& i : animations) {
		auto tmp = fl_parser.GetList(i);
		auto info = fl_parser.GetListInt(tmp[0]); // vel_x, vel_y, 
		auto frames = fl_parser.GetListInt(tmp[1]);
		Velocity v = { (double)info[0], (double)info[1] };
		uint reps = info[2];
		uint delay = info[3];
		this->animations[i] = new FrameListAnimation(i, frames, reps, v, delay);
	}
}

void AnimationHolder::LoadFrameRange(const char* range_path) {
	assert(range_path);
	ConfigParser rng_parser;
	rng_parser.SetNewParser(range_path);
	auto animations = rng_parser.GetList("id");
	for (auto& i : animations) {
		auto tmp = rng_parser.GetListInt(i);
		Velocity v	= {(double) tmp[0], (double) tmp[1]};	
		uint start	= tmp[4];
		uint end	= tmp[5];
		uint reps	= tmp[2];
		uint delay	= tmp[3];
		this->animations[i] = new FrameRangeAnimation(i, start, end, reps, v, delay);
	}
}

void AnimationHolder::LoadTick(const char* tick_path) {
	assert(tick_path);
	ConfigParser t_parser;
	t_parser.SetNewParser(tick_path);
	auto animations = t_parser.GetList("id");
	for (auto& i : animations) {
		auto info = t_parser.GetListInt(i); // reps, delay, discrete
		uint reps = info[0];
		uint delay = info[1];
		bool discrete = info[2];
		this->animations[i] = new TickAnimation(i, delay, reps, discrete);
	}
}

AnimationHolder& AnimationHolder::Get(void) {
	return holder;
}

void AnimationHolder::LoadAll(const char* list_path, const char* range_path, const char* tick_path) {
	LoadFrameList(list_path);
	LoadFrameRange(range_path);
	LoadTick(tick_path);
}

Animation* AnimationHolder::GetAnimation(const std::string& id) {
	auto i = animations.find(id);
	assert(i != animations.end());
	return i->second;
}

#pragma once
#include <list>
#include "VPipe.h"
#include "HPipe.h"

class PipeManager {
private:
	static PipeManager singleton;
	std::list <HPipe*> hpipes;
	std::list <VPipe*> vpipes;
public:
	static PipeManager& GetSingleton(void);
	void AddHPipe(HPipe*);
	void AddVPipe(VPipe*);
	HPipe* GetHPipeCollision(const Rect&) const;
	VPipe* GetVPipeCollision(const Rect&) const;
};


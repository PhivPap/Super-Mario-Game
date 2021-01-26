#include "PipeManager.h"

PipeManager PipeManager::singleton;


PipeManager& PipeManager::GetSingleton(void) {
	return singleton;
}

void PipeManager::AddHPipe(HPipe* pipe) {
	hpipes.push_front(pipe);
}

void PipeManager::AddVPipe(VPipe* pipe) {
	vpipes.push_front(pipe);
}

HPipe* PipeManager::GetHPipeCollision(const Rect& rect) const {
	for (auto i : hpipes) {
		if (i->RectCollidesWithPipe(rect))
			return i;
	}
	return nullptr;
}

VPipe* PipeManager::GetVPipeCollision(const Rect& rect) const {
	for (auto i : vpipes) {
		if (i->RectCollidesWithPipe(rect))
			return i;
	}
	return nullptr;
}
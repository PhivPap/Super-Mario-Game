#include "SpriteManager.h"

SpriteManager SpriteManager::singleton;


void SpriteManager::Add(Sprite* s) {
	auto z_order = s->GetZorder();
	for (auto iter = dpy_list.begin(); iter != dpy_list.end(); ++iter) {
		if (z_order > (*iter)->GetZorder()) {
			dpy_list.insert(iter, s);
			return;
		}
	}
	dpy_list.push_back(s);
}

void SpriteManager::Remove(Sprite* s) {
	dpy_list.remove(s);	
}

const SpriteManager::SpriteList& SpriteManager::GetDisplayList(void) {
	return dpy_list;
}

SpriteManager& SpriteManager::GetSingleton(void) {
	return singleton;
}

const SpriteManager& SpriteManager::GetSingletonConst(void) {
	return singleton;
}
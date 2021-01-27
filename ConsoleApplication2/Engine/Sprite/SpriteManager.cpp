#include "SpriteManager.h"

SpriteManager SpriteManager::singleton;

void SpriteManager::Add(Sprite* s) {
	types[s->GetTypeId()].push_front(s);

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
	types[s->GetTypeId()].remove(s);
	dpy_list.remove(s);
}

const SpriteManager::SpriteList& SpriteManager::GetDisplayList(void) {
	return dpy_list;
}

const SpriteManager::SpriteList& SpriteManager::GetTypeList(const std::string& type_id) {
	return types[type_id];
}

SpriteManager& SpriteManager::GetSingleton(void) {
	return singleton;
}

const SpriteManager& SpriteManager::GetSingletonConst(void) {
	return singleton;
}


void SpriteManager::AddGarbage(Sprite* sprite){
	garbage.push_front(sprite);
}

void SpriteManager::GarbageCollect(){
	while(!garbage.empty()){
		auto* sprite = garbage.front();
		garbage.pop_front();
		delete sprite;
	}
}

#pragma once
#include "Sprite.h"
class SpriteManager final {
public:
	using SpriteList = std::list<Sprite*>;
	using TypeLists = std::map<std::string, SpriteList>;
private:
	SpriteList dpy_list, garbage;
	TypeLists types;
	static SpriteManager singleton;
public:
	void Add(Sprite*);
	void Remove(Sprite*);
	const SpriteList& GetDisplayList(void);
	const SpriteList& GetTypeList(const std::string&);
	static SpriteManager& GetSingleton(void);
	static const SpriteManager& GetSingletonConst(void);
	void AddGarbage(Sprite*);
	void GarbageCollect();
};


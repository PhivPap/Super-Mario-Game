#pragma once
#include "Sprite.h"

class CollisionChecker final {
public:
	using Action = std::function<void(Sprite* s1, Sprite* s2)>;
	
protected:
	static CollisionChecker singleton;
	using Entry = std::tuple<Sprite*, Sprite*, Action>;
	std::list<Entry> entries;
	std::list<Sprite*> garbage;
public:
	void Register(Sprite*, Sprite*, Action);
	void Cancel(Sprite*, Sprite*);
	void Check(void);
	void RemoveAllCollisionWith(Sprite*);
	static CollisionChecker& GetSingleton(void);
	static const CollisionChecker& GetSingletonConst(void);
	void AddGarbage(Sprite*);
	void GarbageCollect();
};


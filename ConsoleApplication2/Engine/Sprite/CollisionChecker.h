#pragma once
#include "Sprite.h"

struct SpriteCouple{
	Sprite* s1;
	Sprite* s2;
};

class CollisionChecker final {
public:
	using Action = std::function<void(Sprite* s1, Sprite* s2)>;
	
protected:
	static CollisionChecker singleton;
	using Entry = std::tuple<Sprite*, Sprite*, Action>;
	std::list<Entry> entries;
	std::list<Sprite*> garbage;
	std::list<SpriteCouple> garbage_sprite_couple;
public:
	void Register(Sprite*, Sprite*, Action);
	void Cancel(Sprite*, Sprite*);
	Action Get(Sprite*, Sprite*);
	void Check(void);
	void RemoveAllCollisionWith(Sprite*);
	static CollisionChecker& GetSingleton(void);
	static const CollisionChecker& GetSingletonConst(void);
	void AddGarbage(Sprite*, Sprite*);
	void AddGarbage(Sprite*);
	void GarbageCollect();
	void DestroyAll();
};


#include "CollisionChecker.h"

CollisionChecker CollisionChecker::singleton; // definition

void CollisionChecker::Register(Sprite* s1, Sprite* s2, Action f) {
	assert(s1 != s2);
	Cancel(s1, s2);
	Cancel(s2, s1);
	entries.push_back(std::make_tuple(s1, s2, f));
	//std::cout << entries.size() << std::endl;
}

CollisionChecker::Action CollisionChecker::Get(Sprite* s1, Sprite* s2) {
	auto i = std::find_if(
		entries.begin(),
		entries.end(),
		[s1, s2](const Entry& e) {
			return	std::get<0>(e) == s1 && std::get<1>(e) == s2 ||
					std::get<0>(e) == s2 && std::get<0>(e) == s1;
		}
	);
	assert(i != entries.end());
	return std::get<2>(*i); //ok m8
}

void CollisionChecker::Cancel(Sprite* s1, Sprite* s2) {
	auto i = std::find_if(
		entries.begin(),
		entries.end(),
		[s1, s2](const Entry& e) {
			return	std::get<0>(e) == s1 && std::get<1>(e) == s2 ||
					std::get<0>(e) == s2 && std::get<0>(e) == s1;
		}
	);
	if(i != entries.end())
		entries.erase(i);
}

void CollisionChecker::Check(void) {
	for (auto& e : entries) {
		if (std::get<0>(e)->CollisionCheck(std::get<1>(e)))
			std::get<2>(e)(std::get<0>(e), std::get<1>(e));
	}
	GarbageCollect();
}

CollisionChecker& CollisionChecker::GetSingleton(void) {
	return singleton;
}

const CollisionChecker& CollisionChecker::GetSingletonConst(void) {
	return singleton;
}

void CollisionChecker::RemoveAllCollisionWith(Sprite* sprite) {
	while (true) { // this is bad ik...
		auto i = std::find_if(
			entries.begin(),
			entries.end(),
			[sprite](const Entry& e) {
				return	(std::get<0>(e) == sprite) || (std::get<1>(e) == sprite);
			}
		);
		if (i != entries.end())
			entries.erase(i);
		else
			break;
	}
}

void CollisionChecker::AddGarbage(Sprite* s1, Sprite* s2) {
	garbage_sprite_couple.push_front({ s1,s2 });
}

void CollisionChecker::AddGarbage(Sprite* sprite){
	garbage.push_front(sprite);
}

void CollisionChecker::GarbageCollect(){
	while(!garbage.empty()){
		auto* sprite = garbage.front();
		garbage.pop_front();
		RemoveAllCollisionWith(sprite);
	}

	while (!garbage_sprite_couple.empty()) {
		auto couple = garbage_sprite_couple.front();
		garbage_sprite_couple.pop_front();
		Cancel(couple.s1, couple.s2);
	}
}


void CollisionChecker::DestroyAll(){
	for(auto it = entries.begin(); it != entries.end(); ){
		it = entries.erase(it);
	}
}
#include "AnimationFilmHolder.h"

AnimationFilmHolder::~AnimationFilmHolder() { CleanUp(); }

const AnimationFilmHolder& AnimationFilmHolder::Get(void) { 
	return holder; 
}

/* Load all films */
void AnimationFilmHolder::LoadAll(const std::string& text) {

}

void AnimationFilmHolder::CleanUp(void) {
	for (auto& i : films)
		delete(i.second);
	films.clear();
}

const AnimationFilm* AnimationFilmHolder::GetFilm(const std::string& id) const {
	auto i = films.find(id);
	return i != films.end() ? i->second : nullptr;
}

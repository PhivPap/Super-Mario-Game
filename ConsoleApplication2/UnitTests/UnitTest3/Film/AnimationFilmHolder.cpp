#include "AnimationFilmHolder.h"

AnimationFilmHolder AnimationFilmHolder::holder; // definition

AnimationFilmHolder::AnimationFilmHolder() {}
AnimationFilmHolder::~AnimationFilmHolder() { CleanUp(); }

/* Load all films from source_path(.data) using parser */
void AnimationFilmHolder::LoadAll(const char* path, const Parser& parser) {
	std::list<AnimationFilm::Data> output;
	parser(output, path);
	assert(!output.empty());

	for (auto& entry : output) {
		assert(!GetFilm(entry.id));
		films[entry.id] = new AnimationFilm(bitmaps.Load(entry.path), entry.rects, entry.id);
	}
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

const BitmapLoader& AnimationFilmHolder::GetBitmapLoader(void) const {
	return bitmaps;
}
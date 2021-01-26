#pragma once
#include "ConfigParser.h"
#include "AnimationFilm.h"

//TODO: move CSV parser here ?

// output	: container of all AnimationFilm Data
// path		: the path to films.data file
static inline void FilmParser(std::list <AnimationFilm::Data>& output, const char* path) {
	assert(path);
	ConfigParser film_parser;
	film_parser.SetNewParser(path);
	auto f = film_parser.GetList("FILMS");
	for (auto& i : f) { // for each film (coins, mario..)
		AnimationFilm::Data data;
		auto d		= film_parser.GetList(i);
		data.id		= d[0];
		data.path	= d[1];
		auto r		= film_parser.GetList(d[2]);

		for (auto& j : r) { // iterate through rects
			data.rects.push_back(film_parser.GetRect(j));
		}

		output.push_back(data);
	}
}

#include <cmath>
template<typename Tnum>
static inline bool isNearlyEqual(Tnum x, Tnum y) {
	const double epsilon = 0.1;
	return std::abs(x - y) <= epsilon;
}
#pragma once
#ifndef TYPES_H
#define TYPES_H

typedef unsigned char byte;
typedef unsigned int uint;
typedef long long unsigned llu;
//typedef ALLEGRO_BITMAP* al_bitmap;

struct Rect {
	uint x, y, w, h;
};

struct Rect_i {
	int x, y, w, h;
};

struct Rect_f {
	float x, y, w, h;
};

struct Point {
	uint x, y;
};

struct Point_f {
	float x, y;
};

struct Dim {
	uint w, h;
};

struct Velocity {
	double x, y;
};

#endif
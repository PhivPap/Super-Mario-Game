#pragma once
#ifndef Util
#define Util

typedef unsigned char byte;
typedef unsigned int uint;
typedef long long unsigned llu;

struct Rect {
	uint x, y, w, h;
};

struct Rect_f {
	float x, y, w, h;
};

struct Point {
	uint x, y;
};

struct Dim {
	uint w, h;
};

struct Movement {
	double x_speed, y_speed;
};

#endif // !Util

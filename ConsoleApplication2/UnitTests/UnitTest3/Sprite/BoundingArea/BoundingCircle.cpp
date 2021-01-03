#include "BoundingCircle.h"
#include "BoundingBox.h"
//#include "BoudingPolygon.h"
#include <math.h>

static inline double DistBetweenPoints(Point_f p1, Point_f p2) {
	auto tmp0 = p1.y - p2.y;
	auto tmp1 = p1.x - p2.x;
	tmp0 *= tmp0;
	tmp1 *= tmp1;
	return sqrt(tmp0 + tmp1);
}

bool BoundingCircle::Intersects(const BoundingBox& box) const {
	auto centers_dist = DistBetweenPoints(box.GetCenter(), {x, y});
	if (centers_dist > r + (box.GetDiagonalLen() / 2))
		return false;
	
	if (y < box.y1) {
		if (box.y1 - y > r)
			return false;
	}
	if (y > box.y2) {
		if (y - box.y2 > r)
			return false;
	}
	if (x > box.x2) {
		if (x - box.x2 > r)
			return false;
	}
	if (x < box.x1) {
		if (box.x1 - x > r)
			return false;
	}
	return true;
}

bool BoundingCircle::Intersects(const BoundingCircle& circle) const {
	auto centers_dist = DistBetweenPoints({ x, y }, { circle.x, circle.y });
	if (centers_dist < r + circle.r)
		return true;
	return false;
}


//bool BoundingCircle::Intersects(const BoundingPolygon&) const {
//
//}


bool BoundingCircle::In(uint x, uint y) const {
	auto points_dist = DistBetweenPoints({this->x, this->y }, {x, y});
	return points_dist < r;
}


bool BoundingCircle::Intersects(const BoundingArea& area) const {
	area.Intersects(*this);
}

BoundingCircle* BoundingCircle::Clone(void) const {
	return new BoundingCircle(x, y, r);
}


BoundingCircle::BoundingCircle(uint x, uint y, uint r) : x(x), y(y), r(r) {}


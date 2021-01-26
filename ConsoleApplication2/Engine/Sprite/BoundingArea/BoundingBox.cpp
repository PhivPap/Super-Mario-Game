#include "BoundingCircle.h"
#include "BoundingBox.h"
#include <math.h>
//#include "BoudingPolygon.h"

bool BoundingBox::Intersects(const BoundingBox& box) const {
	return !(box.x2 < x1 || x2 < box.x1 || box.y2 < y1 || y2 < box.y1);
}

bool BoundingBox::Intersects(const BoundingCircle& circle) const {
	return circle.Intersects(*this);
}

//bool BoundingBox::Intersects(const BoundingPolygon& poly) const {
//	BoundingPolygon::Polygon points;
//	// ....
//}

bool BoundingBox::Intersects(const BoundingArea& area) const {
	return area.Intersects(*this);
}

bool BoundingBox::In(uint x, uint y) const {
	return (x1 <= x && x <= x2 && y1 <= y && y <= y2);
}

void BoundingBox::UpdatePos(uint x, uint y) {
	auto w = x2 - x1;
	auto h = y2 - y1;
	x1 = x;
	y1 = y;
	x2 = x + w;
	y2 = y + h;
}

BoundingBox* BoundingBox::Clone(void) const {
	return new BoundingBox(x1, y1, x2, y2);
}

BoundingBox::BoundingBox(uint x1, uint y1, uint x2, uint y2) : x1(x1), y1(y1), x2(x2), y2(y2) {}

Point_f BoundingBox::GetCenter(void) const{
	return { ((float)(x1 + x2)) / 2, ((float)(y1 + y2)) / 2 };
}

double BoundingBox::GetDiagonalLen(void) const {
	auto tmp0 = x2 - x1;
	auto tmp1 = y2 - y1;
	tmp0 *= tmp0;
	tmp1 *= tmp1;
	return sqrt(tmp0 + tmp1); // VS calling arithmetic overflow. KAPPA
}
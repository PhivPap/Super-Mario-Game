#pragma once
#include "BoundingArea.h"

class BoundingBox : public BoundingArea {
public:
	uint x1, y1, x2, y2;
	virtual bool Intersects(const BoundingBox&) const;
	virtual bool Intersects(const BoundingCircle&) const;
	//virtual bool Intersects(const BoundingPolygon&) const;
	virtual bool In(uint, uint) const;
	virtual bool Intersects(const BoundingArea&) const;
	virtual BoundingBox* Clone(void) const;
	BoundingBox(uint, uint, uint, uint);

	Point_f GetCenter(void) const;
	double GetDiagonalLen(void) const;
};

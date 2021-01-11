#pragma once
#include "BoundingArea.h"

class BoundingCircle : public BoundingArea {
public:
	uint x, y, r;
	virtual bool Intersects(const BoundingBox&) const;
	virtual bool Intersects(const BoundingCircle&) const;
	//virtual bool Intersects(const BoundingPolygon&) const;
	virtual bool In(uint, uint) const;
	virtual bool Intersects(const BoundingArea&) const;
	virtual void UpdatePos(uint, uint);
	virtual BoundingCircle* Clone(void) const;
	BoundingCircle(uint, uint, uint);
};


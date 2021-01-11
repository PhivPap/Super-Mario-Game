#pragma once
#include "Types.h"

//#include "BoundingBox.h"
//#include "BoundingCircle.h"

class BoundingBox;
class BoundingCircle;
class BoundingPolygon;

class BoundingArea{
public:
	virtual bool Intersects(const BoundingBox&) const = 0;
	virtual bool Intersects(const BoundingCircle&) const = 0;
	//virtual bool Intersects(const BoundingPolygon&) const = 0;
	virtual bool In(uint, uint) const = 0;
	virtual bool Intersects(const BoundingArea&) const = 0;
	virtual void UpdatePos(uint, uint) = 0;
	virtual BoundingArea* Clone(void) const = 0;
	virtual ~BoundingArea(); // destructor always virtual.
};


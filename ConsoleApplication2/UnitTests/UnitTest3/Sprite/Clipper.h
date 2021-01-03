#pragma once
#include <functional>
#include "Types.h"

class Clipper {
public: 
	using View = std::function<const Rect& (void)>;
private:
	View view;
public:
	Clipper& SetView(View&);
	bool Clip(const Rect&, const Rect&, Point*, Rect*) const;
};


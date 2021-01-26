#pragma once
#include <functional>
#include "Types.h"

class Clipper {
//public: 
	//using View = std::function<const Rect& (void)>;
private:
	//View view;
	static Rect* view_window;
public:
	//Clipper& SetView(View&);
	static void InitViewWindow(Rect*);
	bool Clip(const Rect&, const Rect&, Point*, Rect_i*) const;
};


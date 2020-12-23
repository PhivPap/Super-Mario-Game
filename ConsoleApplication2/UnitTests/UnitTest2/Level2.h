#pragma once
#include "Level1.h"
#include <allegro5/allegro_primitives.h>



class UnitTest2 : protected UnitTest {
	ALLEGRO_COLOR color;
	Rect rectangle{224, 112, 16, 16 };
	std::vector<std::vector<byte>> grid;
	

	std::function<void(void)> render_rect;
	std::function<void(void)> input_rect;
	
	
	void ReadTextGrid(std::vector<std::vector<byte>>&, Dim&);
	void FilterGridMotion(const Rect&, int&, int&);
	void FilterGridMotionLeft(const Rect&, int&);
	void FilterGridMotionRight(const Rect&, int&);
	void FilterGridMotionUp(const Rect&, int&);
	void FilterGridMotionDown(const Rect&, int&);
	bool CanPassGridTile(Dim, Dim, byte);

public:
	UnitTest2();
	virtual void Initialise(void);
	virtual void Load(void);
	virtual void Clear(void);
	void Main();
};
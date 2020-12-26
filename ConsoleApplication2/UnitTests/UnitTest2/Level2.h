#pragma once
#include "Level1.h"
#include <allegro5/allegro_primitives.h>



class UnitTest2 : protected UnitTest {
	ALLEGRO_COLOR color;
	ALLEGRO_EVENT_QUEUE* keyboard_rect_queue;
	Rect rectangle{224, 176, 16, 16 };
	std::vector<std::vector<byte>> grid;
	

	std::function<void(void)> render_rect;
	std::function<void(void)> input_rect;
	
	
	void ReadTextGrid(std::vector<std::vector<byte>>&, Dim&);
	void FilterGridMotion(const Rect&, int&, int&);
	void FilterGridMotionLeft(const Rect&, int&);
	void FilterGridMotionRight(const Rect&, int&);
	void FilterGridMotionUp(const Rect&, int&);
	void FilterGridMotionDown(const Rect&, int&);
	bool CanPassGridTile(uint, uint, byte);

public:
	UnitTest2();
	virtual void Initialise(void);
	virtual void Load(void);
	virtual void Clear(void);
	void Main();
};
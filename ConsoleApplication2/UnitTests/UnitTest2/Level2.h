#pragma once
#include "Level1.h"
#include <allegro5/allegro_primitives.h>

struct Movement {
	double x_speed, y_speed;
};

class UnitTest2 : protected UnitTest {
	ALLEGRO_COLOR color;
	ALLEGRO_EVENT_QUEUE* keyboard_rect_queue;
	Rect rectangle{224, 176, 16, 16 };
	Movement rect_mvmnt{ 0,0 };
	bool movement_keys[ALLEGRO_KEY_MAX] = { 0 };
	std::vector<std::vector<byte>> grid; //grid[row][col] 
	

	std::function<void(void)> render_rect;
	std::function<void(void)> input_rect;
	std::function<void(void)> physics_rect;
	
	
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
#pragma once
#include "Level1.h"
#include <allegro5/allegro_primitives.h>

#define RECT_MAX_SPEED_X 120 // pixels per second. (p/s)
#define RECT_MAX_SPEED_Y 120  // pixels per second. (p/s)
#define RECT_UPDATE_POS 0.00834 // 120 times per second.

struct Movement {
	double x_speed, y_speed;
};

class UnitTest2 : protected UnitTest {
	ALLEGRO_COLOR color;
	ALLEGRO_EVENT_QUEUE* keyboard_rect_queue;
	ALLEGRO_TIMER* rect_pos_timer;
	
	std::vector<std::vector<byte>> grid; //grid[row][col] 
	bool rect_filled = true;

	void ReadTextGrid(std::vector<std::vector<byte>>&, Dim&);
	void FilterGridMotionLeft(const Rect&, int&);
	void FilterGridMotionRight(const Rect&, int&);
	void FilterGridMotionUp(const Rect&, int&);
	void FilterGridMotionDown(const Rect&, int&);
	bool CanPassGridTile(uint, uint, byte);

protected:
	Movement rect_mvmnt{ 0,0 };
	Rect rectangle{ 224, 176, 16, 16 };
	bool movement_keys[ALLEGRO_KEY_MAX] = { 0 };
	ALLEGRO_EVENT_QUEUE* timer_queue0;
	std::function<void(void)> render_rect;
	std::function<void(void)> input_rect;
	std::function<void(void)> physics_rect;	

	void FilterGridMotion(const Rect&, int&, int&);

public:
	UnitTest2();
	virtual void Initialise(void);
	virtual void Load(void);
	virtual void Clear(void);
	void Main();
};
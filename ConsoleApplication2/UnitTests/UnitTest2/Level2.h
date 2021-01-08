#pragma once
#include "Level1.h"
#include <allegro5/allegro_primitives.h>
#include <assert.h>

#define GRID_ELEMENT_WIDTH 4 // in pixels
#define GRID_ELEMENT_HEIGHT 4 // in pixels

#if TILE_WIDTH % GRID_ELEMENT_WIDTH != 0
#error "TILE_WIDTH % GRID_ELEMENT_WIDTH must be zero!"
#endif

#if TILE_HEIGHT % GRID_ELEMENT_HEIGHT != 0
#error "TILE_HEIGHT % GRID_ELEMENT_HEIGHT must be zero!"
#endif

#define GRID_BLOCK_COLUMNS		(TILE_WIDTH / GRID_ELEMENT_WIDTH)
#define GRID_BLOCK_ROWS			(TILE_HEIGHT / GRID_ELEMENT_HEIGHT)
#define GRID_ELEMENTS_PER_TILE	(GRID_BLOCK_ROWS * GRID_BLOCK_COLUMNS)
#define GRID_MAX_HEIGHT			(MAX_HEIGHT * GRID_BLOCK_ROWS)
#define GRID_MAX_WIDTH			(MAX_WIDTH * GRID_BLOCK_COLUMNS)

#define GRID_EMPTY_TILE 0
#define GRID_SOLID_TILE 1

#define RECT_MAX_SPEED_X 150 // pixels per second. (p/s)
#define RECT_MAX_SPEED_Y 150  // pixels per second. (p/s)
#define RECT_UPDATE_POS 0.00834 // 120 times per second.

static_assert(RECT_MAX_SPEED_X * RECT_UPDATE_POS < GRID_ELEMENT_WIDTH, "Increase pos update frequency or decrease MAX_X_SPEED.\n");
static_assert(RECT_MAX_SPEED_Y* RECT_UPDATE_POS < GRID_ELEMENT_HEIGHT, "Increase pos update frequency or decrease MAX_Y_SPEED.\n");


class UnitTest2 : protected UnitTest {
	ALLEGRO_COLOR color;
	ALLEGRO_EVENT_QUEUE* keyboard_rect_queue;
	ALLEGRO_TIMER* rect_pos_timer;
	
	std::vector<std::vector<byte>> grid; //grid[row][col] 
	bool rect_filled = true;

	void ReadTextGrid(std::vector<std::vector<byte>>&, Dim&);
	void FilterGridMotionLeft(const Rect_f&, float&);
	void FilterGridMotionRight(const Rect_f&, float&);
	void FilterGridMotionUp(const Rect_f&, float&);
	void FilterGridMotionDown(const Rect_f&, float&);
	bool CanPassGridTile(uint, uint, byte);

protected:
	Velocity rect_mvmnt{ 0,0 };
	Rect_f rectangle{ 0, 176, 16, 16 };
	bool movement_keys[ALLEGRO_KEY_MAX] = { 0 };
	ALLEGRO_EVENT_QUEUE* timer_queue0;
	std::function<void(void)> render_rect;
	std::function<void(void)> input_rect;
	std::function<void(void)> physics_rect;	

	void FilterGridMotion(const Rect_f&, float&, float&);

public:
	UnitTest2();
	virtual void Initialise(void);
	virtual void Load(void);
	virtual void Clear(void);
	void Main();
};
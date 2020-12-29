#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <vector> 
#include <fstream>
#include <chrono>

#include "Engine.h"

#define DIS_WIDTH 1440
#define DIS_HEIGHT 464 // cannot be 480, but Y??
#define TILE_WIDTH 16
#define TILE_HEIGHT 16

#define SCROLL_DIST 64

#define TILESET_PATH "UnitTests/UnitTest3/media/super_mario_tiles.png"
#define TILEMAP_PATH "UnitTests/UnitTest3/media/mario_map_0.csv"

typedef unsigned char byte;
typedef unsigned int uint;
typedef long long unsigned llu;

struct Rect {
	uint x, y, w, h;
};

struct Point {
	uint x, y;
};

struct Dim {
	uint w, h;
};

class UnitTest : protected app::App {
	ALLEGRO_DISPLAY* display;
	ALLEGRO_BITMAP* tileset;
	ALLEGRO_BITMAP* tile_alligned;
	ALLEGRO_EVENT_QUEUE* mouse_queue;
	ALLEGRO_TIMER* timer;
	std::vector<std::vector<byte>> map;
	uint tileset_width;
	
	std::chrono::steady_clock::time_point old_time, new_time;
	llu frames;
	uint counter;
	Rect tile_view_win;
	
	std::function<bool(void)> done;

	
	void Scroll(int, int);
	
	void FilterScroll(int&, int&);
	void TileTerrainDisplay();
	void ReadTextMap(std::vector<std::vector<byte>>&, Dim&);

protected:
	std::function<void(void)> render_terrain;
	std::function<void(void)> flip_display;
	std::function<void(void)> input_events0;
	std::function<void(void)> input_scroll;
	void ScrollWithBoundsCheck(int, int, bool&);
	ALLEGRO_EVENT_QUEUE* keyboard_queue;
	ALLEGRO_EVENT_QUEUE* display_queue;
	ALLEGRO_EVENT display_event, kb_event, mouse_event;
	bool display_event_b, kb_event_b, mouse_event_b;
	bool game_finished;
	Dim map_dim;
	Rect view_win;
	bool tile_win_moved;

	bool TileAllignedViewBoundCheck();
	static void FilterScrollDistance(uint, uint, int&, uint);

public:
	static int ReadCSV(std::vector<std::vector<byte>>&, const char*); //csv to byte array.
	std::vector<std::vector<byte>> &getMapRef();
	ALLEGRO_BITMAP* getTileset() const;
	UnitTest();
	virtual void Initialise(void);
	virtual void Load(void);
	virtual void Clear(void);
	void Main();
};

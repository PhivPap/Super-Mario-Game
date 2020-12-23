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

#define DIS_WIDTH 1280
#define DIS_HEIGHT 960
#define TILE_WIDTH 16
#define TILE_HEIGHT 16

#define SCROLL_DIST 64

#define TILESET_PATH "UnitTests/UnitTest1/media/overworld_tileset_grass.png"
#define TILEMAP_PATH "UnitTests/UnitTest1/media/my_map.csv"

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
	ALLEGRO_EVENT_QUEUE* display_queue;
	ALLEGRO_EVENT_QUEUE* mouse_queue;
	ALLEGRO_TIMER* timer;
	std::vector<std::vector<byte>> map;
	uint tileset_width;
	bool game_finished;
	bool tile_win_moved;
	std::chrono::steady_clock::time_point old_time, new_time;
	llu frames;
	uint counter;
	Rect view_win;
	Rect tile_view_win;
	

	std::function<void(void)> render_terrain;
	std::function<void(void)> flip_display;
	std::function<void(void)> input_scroll;
	std::function<bool(void)> done;

	bool TileAllignedViewBoundCheck();
	void ScrollWithBoundsCheck(int, int, bool&);
	void Scroll(int, int);
	static void FilterScrollDistance(uint, uint, int&, uint);
	void FilterScroll(int&, int&);
	void TileTerrainDisplay() const;
	void ReadTextMap(std::vector<std::vector<byte>>&, Dim&);

protected:
	ALLEGRO_EVENT_QUEUE* keyboard_queue;
	Dim map_dim;
public:
	static int ReadCSV(std::vector<std::vector<byte>>&, const char*); //csv to byte array.
	std::vector<std::vector<byte>> &getMapRef();
	UnitTest();
	virtual void Initialise(void);
	virtual void Load(void);
	virtual void Clear(void);
	void Main();
};

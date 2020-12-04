#include <iostream>
#include <fstream>
#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <vector> 
#include <fstream>
#include <chrono>

#include "Engine.h"

#define DIS_WIDTH 640
#define DIS_HEIGHT 480
#define TILE_WIDTH 16
#define TILE_HEIGHT 16
#define VIEW_WINS_HEIGHT_DISPLACEMENT 16
#define VIEW_WINS_WIDTH_DISPLACEMENT 16

#define SCROLL_DIST 8

#define TILESET_PATH "UnitTest1Media/overworld_tileset_grass.png"
#define TILEMAP_PATH "UnitTest1Media/map1_Kachelebene_1.csv"

//#define DIS_WIDTH_AS_TILES (DIS_WIDTH/TILE_WIDTH)
//#define DIS_HEIGHT_AS_TILES (DIS_HEIGHT/TILE_HEIGHT)

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

class UnitTest : app::App {

	ALLEGRO_DISPLAY* display = 0;
	ALLEGRO_BITMAP* tileset = 0;
	ALLEGRO_BITMAP* tile_alligned = 0;
	ALLEGRO_EVENT_QUEUE* display_queue = 0;
	ALLEGRO_EVENT_QUEUE* keyboard_queue = 0;
	ALLEGRO_EVENT_QUEUE* mouse_queue = 0;
	ALLEGRO_TIMER* timer = 0;
	std::vector<std::vector<byte>> map;
	uint tileset_width = 0;
	bool game_finished = false;
	bool tile_win_moved = true;
	std::chrono::steady_clock::time_point old_time, new_time;
	long long unsigned frames = 0;
	uint counter = 0;
	Rect view_win = { 0, 0, DIS_WIDTH, DIS_HEIGHT };
	Rect tile_view_win = { 0, 0, DIS_WIDTH + VIEW_WINS_WIDTH_DISPLACEMENT, DIS_HEIGHT + VIEW_WINS_HEIGHT_DISPLACEMENT};
	Dim map_dim = {0};

	std::function<void(void)> render = [&] {
		frames++;
		
		TileTerrainDisplay();
	};

	std::function<void(void)> input = [&] {
		ALLEGRO_EVENT display_event;
		ALLEGRO_EVENT kb_event;
		ALLEGRO_EVENT mouse_event;

		if (al_get_next_event(display_queue, &display_event)) {
			if (display_event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
				game_finished = true;
		}

		tile_win_moved = false;
		if (al_get_next_event(keyboard_queue, &kb_event)) {
			if (kb_event.type == ALLEGRO_EVENT_KEY_DOWN) {
				if (kb_event.keyboard.keycode == ALLEGRO_KEY_UP) {
					//std::cout << "up key" << std::endl;
					ScrollWithBoundsCheck(0, -SCROLL_DIST, tile_win_moved);
				}
				else if (kb_event.keyboard.keycode == ALLEGRO_KEY_DOWN) {
					//std::cout << "down key" << std::endl;
					ScrollWithBoundsCheck(0, SCROLL_DIST, tile_win_moved);
				}
				else if (kb_event.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
					//std::cout << "right key" << std::endl;
					ScrollWithBoundsCheck(SCROLL_DIST, 0, tile_win_moved);
				}
				else if (kb_event.keyboard.keycode == ALLEGRO_KEY_LEFT) {
					//std::cout << "left key" << std::endl;
					ScrollWithBoundsCheck(-SCROLL_DIST, 0, tile_win_moved);
				}
				else if (kb_event.keyboard.keycode == ALLEGRO_KEY_HOME) {
					//al_set_window_position(display, 0, 0);
				}
				else if (kb_event.keyboard.keycode == ALLEGRO_KEY_END) {

				}
			}
		}
		if (al_get_next_event(mouse_queue, &mouse_event)) {
			if (mouse_event.mouse.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
				if (mouse_event.mouse.button == 1) {
					int dx, dy;
					al_get_mouse_cursor_position(&dx, &dy);
					std::cout << dx << " " << dy << std::endl;
					ScrollWithBoundsCheck(dx, dy, tile_win_moved);
				}
			}
		}
	};


	std::function<bool(void)> done = [=] {
		return game_finished;
	};
	
	bool TileAllignedViewBoundCheck() {
		bool moved = false;
		if (view_win.x < tile_view_win.x) {
			tile_view_win.x -= VIEW_WINS_WIDTH_DISPLACEMENT;
			moved = true;
		}
		if (view_win.y < tile_view_win.y) {
			tile_view_win.y -= VIEW_WINS_HEIGHT_DISPLACEMENT;
			moved = true;
		}
		if (view_win.x + view_win.w > tile_view_win.x + tile_view_win.w){
			tile_view_win.x += VIEW_WINS_WIDTH_DISPLACEMENT;
			moved = true;
		}
		if (view_win.y + view_win.h > tile_view_win.y + tile_view_win.h){
			tile_view_win.y += VIEW_WINS_HEIGHT_DISPLACEMENT;
			moved = true;
		}
		return moved;
	}

	void ScrollWithBoundsCheck(int dx, int dy, bool& tile_win_moved) {
		FilterScroll(dx, dy);
		Scroll(dx, dy);
		tile_win_moved = TileAllignedViewBoundCheck();
	}
	
	void Scroll(int dx, int dy) {
		view_win.x += dx;
		view_win.y += dy;
	}

	static void FilterScrollDistance(uint viewStartCoord, uint viewSize, int& d, uint map_dim) {
		int val = d + viewStartCoord;
		if (val < 0)
			d = viewStartCoord;
			//d = 0;
		else if ((val + viewSize) >= map_dim)
			d = map_dim - (viewStartCoord + viewSize);
			//d = map_dim - viewSize;
	}

	void FilterScroll(int& dx, int& dy) {
		FilterScrollDistance(view_win.x, view_win.w, dx, map_dim.w);
		FilterScrollDistance(view_win.y, view_win.h, dy, map_dim.h);
	}

	void TileTerrainDisplay() const {
		if(tile_win_moved){
			auto start_row = tile_view_win.y / TILE_HEIGHT;
			auto start_col = tile_view_win.x / TILE_WIDTH;
			auto end_row = (tile_view_win.y + tile_view_win.h - 1) / TILE_HEIGHT;
			auto end_col = (tile_view_win.x + tile_view_win.w - 1) / TILE_WIDTH;
			uint dest_row = 0, dest_col = 0;
			for (auto row = start_row; row <= end_row; row++) {
				for(auto col = start_col; col <= end_col; col++) {
					// source is map, tile bitmap
					// draw to tile alligned bitmap
					al_set_target_bitmap(tile_alligned);
					al_draw_bitmap_region(
						tileset,
						(map[row][col] % tileset_width) * TILE_WIDTH,
						(uint)(map[row][col] / tileset_width) * TILE_HEIGHT,
						TILE_WIDTH,
						TILE_HEIGHT,
						dest_col * TILE_WIDTH,
						dest_row * TILE_HEIGHT,
						0);
						dest_col++;
				}
				dest_col = 0;
				dest_row++;
			}
			al_set_target_backbuffer(display);
		}

		al_draw_bitmap_region(
						tile_alligned,
						view_win.x - tile_view_win.x,
						view_win.y - tile_view_win.y,
						DIS_WIDTH,
						DIS_HEIGHT,
						0,
						0,
						0);
		al_flip_display();
	}

	void ReadTextMap(std::vector<std::vector<byte>>& map, Dim& map_dim) {
		std::ifstream map_file(TILEMAP_PATH);
		std::string line;
		std::string token = "";
		while (getline(map_file, line)) {
			std::vector<byte> row;
			auto i = 0;
			auto line_len = line.length();
			while (i < line_len) {
				if (line[i] == ',') {
					row.push_back((byte)std::stoi(token, nullptr));
					token.clear();
					i++;
					continue;
				}
				token += line[i++];
			}
			if (!token.empty()) {
				row.push_back((byte)std::stoi(token, nullptr));
				token.clear();
			}
			map.push_back(row);
		}
		map_dim.w = (uint)map[0].size() * TILE_WIDTH;
		map_dim.h = (uint)map.size() * TILE_HEIGHT;
		map_file.close();
	}

public:
	virtual void Initialise(void) {
		al_init();
		al_init_image_addon(); //bitmaps, pngs, jpegs etc
		al_install_mouse();
		al_install_keyboard();
		display_queue = al_create_event_queue();
		keyboard_queue = al_create_event_queue();
		mouse_queue = al_create_event_queue();
		display = al_create_display(DIS_WIDTH, DIS_HEIGHT);
		al_register_event_source(display_queue, al_get_display_event_source(display));
		al_register_event_source(keyboard_queue, al_get_keyboard_event_source());
		al_register_event_source(mouse_queue, al_get_mouse_event_source());
		tileset = al_load_bitmap(TILESET_PATH);
		tileset_width = al_get_bitmap_width(tileset) / TILE_WIDTH;
		tile_alligned = al_create_bitmap(tile_view_win.w, tile_view_win.h);
	}
	virtual void Load(void) {
		ReadTextMap(map, map_dim);
		game.SetRender(render);
		game.SetDone(done);
		game.SetInput(input);
		old_time = std::chrono::high_resolution_clock::now();
	}

	virtual void Clear(void) {
		new_time = std::chrono::high_resolution_clock::now();
		std::cout << (double)frames / (double)((double)std::chrono::duration_cast<std::chrono::microseconds>(new_time - old_time).count() / 1000000.0f) << std::endl;
		al_destroy_bitmap(tileset);
		al_destroy_bitmap(tile_alligned);
		al_destroy_display(display);
		al_uninstall_keyboard();
		al_uninstall_mouse();
	}
	void Main() {
		app::App::Main();
	}
};

int main() {
	UnitTest utest;
	utest.Main();
}
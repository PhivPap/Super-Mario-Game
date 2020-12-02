#include <iostream>
#include <fstream>
#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <vector> 
#include <fstream>

#include "Engine.h"

#define DIS_WIDTH 640
#define DIS_HEIGHT 480
#define TILE_WIDTH 16
#define TILE_HEIGHT 16

#define TILESET_PATH "UnitTest1Media/overworld_tileset_grass.png"
#define TILEMAP_PATH "UnitTest1Media/map1_Kachelebene 1.csv"

//#define DIS_WIDTH_AS_TILES (DIS_WIDTH/TILE_WIDTH)
//#define DIS_HEIGHT_AS_TILES (DIS_HEIGHT/TILE_HEIGHT)

typedef unsigned char byte;
typedef unsigned int uint;

class UnitTest : app::App {

	ALLEGRO_DISPLAY* display = 0;
	ALLEGRO_BITMAP* tileset = 0;
	ALLEGRO_EVENT_QUEUE* queue = 0;
	std::vector<byte> map;
	uint tile_set_width = 0;
	uint map_width = 0;

	std::function<void(void)> render = [&] {
		auto size_of_map = map.size();
		for (auto i = 0; i < size_of_map; i++) {
			al_draw_bitmap_region(tileset,
				(map[i] % tile_set_width) * TILE_WIDTH,
				(uint)(map[i] / tile_set_width) * TILE_HEIGHT,
				TILE_WIDTH,
				TILE_HEIGHT,
				(i % map_width) * TILE_WIDTH,
				(uint)(i / map_width) * TILE_HEIGHT,
				0);
		}
		al_flip_display();
		
	};

	std::function<bool(void)> done = [&] {
		ALLEGRO_EVENT event;
		//https://www.allegro.cc/manual/5/al_wait_for_event_timed
		if (al_wait_for_event_timed(queue, &event, 0.001))
			if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
				return true;
		return false;
	};

	void ReadTextMap(std::vector<byte>& map, uint& map_width) {
		std::ifstream map_file(TILEMAP_PATH);
		std::string line;
		std::string token = "";
		uint files_lines = 0;
		while (getline(map_file, line)) {
			files_lines++;
			auto i = 0;
			auto line_len = line.length();
			while (i < line_len) {
				if (line[i] == ',') {
					map.push_back((byte)std::stoi(token, nullptr));
					token.clear();
					i++;
					continue;
				}
				token += line[i++];
			}
			if (!token.empty()) {
				map.push_back((byte)std::stoi(token, nullptr));
				token.clear();
			}
		}
		map_width = (uint)map.size() / files_lines;
		map_file.close();
	}

public:
	virtual void Initialise(void) {
		al_init();
		al_init_image_addon(); //bitmaps, pngs, jpegs etc
		al_install_mouse();
		queue = al_create_event_queue();
		display = al_create_display(DIS_WIDTH, DIS_HEIGHT);
		al_register_event_source(queue, al_get_display_event_source(display));
		tileset = al_load_bitmap(TILESET_PATH);
		tile_set_width = al_get_bitmap_width(tileset) / TILE_WIDTH;
	}
	virtual void Load(void) {
		ReadTextMap(map, map_width);
		game.SetRender(render);
		game.SetDone(done);
		/*std::cout << "Load\n";*/
	}

	virtual void Clear(void) {
		al_destroy_bitmap(tileset);
		al_destroy_display(display);
		/*std::cout << "Clear\n";*/
	}
	void Main() {
		/*std::cout << sizeof(game) << std::endl;*/
		app::App::Main();
	}
};


int main(){
	UnitTest utest;
	utest.Main();
	// testing here -----------------------------------------------------
}
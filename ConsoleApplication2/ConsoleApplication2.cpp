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

//#define DIS_WIDTH_AS_TILES (DIS_WIDTH/TILE_WIDTH)
//#define DIS_HEIGHT_AS_TILES (DIS_HEIGHT/TILE_HEIGHT)

typedef unsigned char byte;
typedef unsigned int uint;

class UnitTest : app::App {
public:
	virtual void Initialise(void) {
		/*std::cout << "Initialise\n";*/
	}
	virtual void Load(void) {
		/*std::cout << "Load\n";*/
	}
	virtual void Run(void) { 
		/*std::cout << "Run\n";*/
	}
	virtual void RunIteration(void) {
	}
	virtual void Clear(void) {
		/*std::cout << "Clear\n";*/
	}
	void Main() {
		/*std::cout << sizeof(game) << std::endl;*/
		app::App::Main();
	}
};



void draw_map(ALLEGRO_BITMAP* tile_set, std::vector<byte> &map, uint tile_set_width, uint map_width) {
	//al_draw_bitmap_region(tiles_bitmap, TILE_SIZE * map[i], 0, TILE_SIZE, TILE_SIZE, i * TILE_SIZE, 0, 0);
	auto size_of_map = map.size();
	for (auto i = 0; i < size_of_map; i++) {
		al_draw_bitmap_region(	tile_set, 
								(map[i] % tile_set_width) * TILE_WIDTH,
								(uint)(map[i] / tile_set_width) * TILE_HEIGHT, 
								TILE_WIDTH,
								TILE_HEIGHT,
								(i % map_width) * TILE_WIDTH,
								(uint)(i / map_width) * TILE_HEIGHT,
								0);
	}
}

void load_map(std::vector<byte>& map, uint &map_width) {
	std::ifstream map_file("UnitTest1Media/map1_Kachelebene 1.csv");
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
	map_width = map.size() / files_lines;
	map_file.close();
}

int main(){
	UnitTest utest;
	utest.Main();
	// testing here -----------------------------------------------------
	
	al_init();
	al_init_image_addon(); //bitmaps, pngs, jpegs etc
	/* mario is 16x32  */
	ALLEGRO_DISPLAY* display = al_create_display(DIS_WIDTH, DIS_HEIGHT); // remember destroy
	ALLEGRO_BITMAP* tile_set = al_load_bitmap("UnitTest1Media/overworld_tileset_grass.png"); // remember destroy
	uint tile_set_width = al_get_bitmap_width(tile_set) / TILE_WIDTH;
	//uint tile_set_height = al_get_bitmap_height(tile_set) / TILE_HEIGHT;


	std::vector<byte> map;
	uint map_width;
	load_map(map, map_width);
	draw_map(tile_set, map, tile_set_width, map_width);
	al_flip_display();
	al_rest(10.0);


}
#include "ComputeTileGridBlocks.h"
#include "Level1.h"

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ These are from Lecture 8: Slide 16. 

#define GRID_ELEMENT_WIDTH 16 // in pixels
#define GRID_ELEMENT_HEIGHT 16 // in pixels

#define GRID_BLOCK_COLUMNS		(TILE_WIDTH / GRID_ELEMENT_WIDTH)		// 16/4 = 4
#define GRID_BLOCK_ROWS			(TILE_HEIGHT / GRID_ELEMENT_HEIGHT)		// 16/4 = 4
#define GRID_ELEMENTS_PER_TILE	(GRID_BLOCK_ROWS * GRID_BLOCK_COLUMNS)	// <-- ?? 
/*
_________
|_|_|_|_|
|_|_|_|_|
|_|_|_|_|
|_|_|_|_|

*/


// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ These are from Lecture 8: Slide 33.

#define GRID1_OUT "UnitTests\\UnitTest2\\Grid\\grid1.csv"
#define GRID2_OUT "UnitTests\\UnitTest2\\Grid\\grid2.csv"
//#define TILES_AMOUNT 252

#define GRID_EMPTY_TILE 0
#define GRID_SOLID_TILE 1


uint MAX_HEIGHT = -1;
uint MAX_WIDTH = -1;
uint GRID_MAX_HEIGHT = -1;
uint GRID_MAX_WIDTH = -1;

byte tileset_info_1[] = {
	#include "TileInfo1.txt"
};

// Inits grid to the correct size and the 2 vars above.
static void Init(std::vector<std::vector<byte>> &map, std::vector<std::vector<byte>> &grid) {
	MAX_HEIGHT = (uint)map.size();
	MAX_WIDTH = (uint)map[0].size();
	GRID_MAX_HEIGHT = MAX_HEIGHT * GRID_BLOCK_ROWS;
	GRID_MAX_WIDTH = MAX_WIDTH * GRID_BLOCK_COLUMNS;
	std::cout << "GRID_MAX_HEIGHT = " << GRID_MAX_HEIGHT << std::endl << "GRID_MAX_WIDTH = " << GRID_MAX_WIDTH << std::endl;
	for (uint i = 0; i < GRID_MAX_HEIGHT; i++) {
		std::vector<byte> row(GRID_MAX_WIDTH);
		grid.push_back(row);
	}
}

//this is will print ',' before the newline. Can be fixed by printing to an std::stringstream first.
static void WriteGrid(std::vector<std::vector<byte>>& grid, const char *fileName) {
	std::ofstream grid_file(fileName);
	for (auto i : grid) {
		for (auto j : i) {
			grid_file << (uint)j;
			grid_file << ",";
		}
		grid_file << "\n";
	}
	grid_file.close();
}


static void ComputeTileGridBlocks1(std::vector<std::vector<byte>>& map, std::vector<std::vector<byte>> &grid) {
	uint i = 0;
	uint grid_elem_x;
	uint grid_elem_y = 0;
	for (uint tile_y = 0; tile_y < MAX_HEIGHT; tile_y++) {
		grid_elem_x = 0;
		for (uint tile_x = 0; tile_x < MAX_WIDTH; tile_x++) {
			grid[grid_elem_y][grid_elem_x] = tileset_info_1[map[tile_y][tile_x]];
			grid_elem_x += GRID_BLOCK_COLUMNS;
		}
		grid_elem_y += GRID_BLOCK_ROWS;
	}
}

static void ComputeTileGridBlocks2 (
			std::vector<std::vector<byte>>& map,
			std::vector<std::vector<byte>>& grid,
			ALLEGRO_BITMAP* tileset,
			ALLEGRO_COLOR color,
			byte solid_threshold
) {
	auto tile_elem = al_create_bitmap(TILE_WIDTH, TILE_HEIGHT);
	auto grid_elem = al_create_bitmap(GRID_ELEMENT_WIDTH, GRID_ELEMENT_HEIGHT);
	for(auto row = 0; row < MAX_HEIGHT; row++)
		for (auto col = 0; row < MAX_WIDTH; col++) {
			//TODO
		}
	al_destroy_bitmap(tile_elem);
	al_destroy_bitmap(grid_elem);
}

void ComputeTileGridBlocks() {
	UnitTest utest;
	utest.Load(); // to load map
	std::vector<std::vector<byte>> map = utest.getMapRef(); // this is a ref.
	std::vector<std::vector<byte>> grid;
	Init(map, grid);


	ComputeTileGridBlocks1(map, grid);
	WriteGrid(grid, GRID1_OUT);
	/*ComputeTileGridBlocks2();
	WriteGrid(grid, GRID2_OUT);*/
}
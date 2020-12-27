#include "ComputeTileGridBlocks.h"
#include "Level1.h"

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ These are from Lecture 8: Slide 16. 

#define GRID_ELEMENT_WIDTH 4 // in pixels
#define GRID_ELEMENT_HEIGHT 4 // in pixels

#define GRID_BLOCK_COLUMNS		(TILE_WIDTH / GRID_ELEMENT_WIDTH)		// 16/4 = 4
#define GRID_BLOCK_ROWS			(TILE_HEIGHT / GRID_ELEMENT_HEIGHT)		// 16/4 = 4
#define GRID_ELEMENTS_PER_TILE	(GRID_BLOCK_ROWS * GRID_BLOCK_COLUMNS)	// <-- ?? 


#define TILESET_WIDTH 12
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

byte tileset_info[] = {
	#include "TileInfo1.txt"
};

// Inits grid to the correct size and the 2 vars above.
static void Init(std::vector<std::vector<byte>> &map, std::vector<std::vector<byte>> &grid) {
	MAX_HEIGHT = (uint)map.size();
	MAX_WIDTH = (uint)map[0].size();
	GRID_MAX_HEIGHT = MAX_HEIGHT * GRID_BLOCK_ROWS;
	GRID_MAX_WIDTH = MAX_WIDTH * GRID_BLOCK_COLUMNS;
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
	std::cout << "writegrid" << std::endl;
	grid_file.close();
}

static bool ComputeIsGridIndexEmpty(ALLEGRO_BITMAP* grid_elem, const ALLEGRO_COLOR& trans_color, byte solid_threshold) {
	//al_lock_bitmap(grid_elem, ,ALLEGRO_LOCK_READONLY);
	auto n = 0;
	for (auto y = 0; y < GRID_ELEMENT_HEIGHT; y++) {
		for (auto x = 0; x < GRID_ELEMENT_WIDTH; x++) {
			auto color = al_get_pixel(grid_elem, x, y);
			if (memcmp(&color, &trans_color, sizeof(ALLEGRO_COLOR)) != 0) // diff colors
				n++;
		}
	}
	return n <= solid_threshold;
}


static void ComputeGridBlock(uint grid_row, uint grid_col, std::vector<std::vector<byte>> &grid, 
					uint tileset_index, 
					ALLEGRO_BITMAP* grid_elem, 
					ALLEGRO_BITMAP* tileset, 
					const ALLEGRO_COLOR& trans_color, 
					byte solid_threshold){

	uint tileset_x = (tileset_index % TILESET_WIDTH) * TILE_WIDTH;
	uint tileset_y = (uint)(tileset_index / TILESET_WIDTH) * TILE_HEIGHT;

	for (uint row = 0; row < GRID_BLOCK_ROWS; row++) {
		for (uint col = 0; col < GRID_BLOCK_COLUMNS; col++) {
			al_set_target_bitmap(grid_elem);
			al_draw_bitmap_region(
				tileset,
				tileset_x + col * GRID_ELEMENT_WIDTH,
				tileset_y + row * GRID_ELEMENT_HEIGHT,
				GRID_ELEMENT_WIDTH,
				GRID_ELEMENT_HEIGHT,
				0, 0, 0);
			/*al_flip_display();
			al_rest(10);*/
			auto is_empty = ComputeIsGridIndexEmpty(grid_elem, trans_color, solid_threshold);
			grid[(llu)grid_row + (llu)row][(llu)grid_col + (llu)col] = is_empty ? GRID_EMPTY_TILE : GRID_SOLID_TILE;
		}
	}
}


static void ComputeTileGridBlocks1(std::vector<std::vector<byte>>& map, std::vector<std::vector<byte>> &grid) {
	uint i = 0;
	uint grid_elem_x;
	uint grid_elem_y = 0;
	for (uint tile_y = 0; tile_y < MAX_HEIGHT; tile_y++) {
		grid_elem_x = 0;
		for (uint tile_x = 0; tile_x < MAX_WIDTH; tile_x++) {
			byte tile_has_collision = tileset_info[map[tile_y][tile_x]];
			//grid[grid_elem_y][grid_elem_x] = tileset_info[map[tile_y][tile_x]];
			for (uint x = grid_elem_x; x < grid_elem_x + GRID_BLOCK_COLUMNS; x++) {
				for (uint y = grid_elem_y; y < grid_elem_y + GRID_BLOCK_ROWS; y++) {
					grid[y][x] = tile_has_collision;
				}
			}
			grid_elem_x += GRID_BLOCK_COLUMNS;
		}
		grid_elem_y += GRID_BLOCK_ROWS;
	}
}



static void ComputeTileGridBlocks2 (
			std::vector<std::vector<byte>>& map,
			std::vector<std::vector<byte>>& grid,
			ALLEGRO_BITMAP* tileset,
			ALLEGRO_COLOR trans_color,
			byte solid_threshold
) {
	auto tile_elem = al_create_bitmap(TILE_WIDTH, TILE_HEIGHT);
	auto grid_elem = al_create_bitmap(GRID_ELEMENT_WIDTH, GRID_ELEMENT_HEIGHT);
	uint grid_col = 0;
	uint grid_row = 0;
	for (uint map_row = 0; map_row < MAX_HEIGHT; map_row++) {
		grid_col = 0;
		for (uint map_col = 0; map_col < MAX_WIDTH; map_col++) {
			if (tileset_info[map[map_row][map_col]] == 0) {		// if tile has no collisions
				for (uint i = grid_col; i < grid_col + GRID_BLOCK_COLUMNS; i++) {
					for (uint j = grid_row; j < grid_row + GRID_BLOCK_ROWS; j++) {
						grid[grid_row][grid_col] = 0;		// ALL GRID ELEMENTS OF TILE ARE 0.
					}
				}
			}
			else { // tile has some collisions
				ComputeGridBlock(grid_row, grid_col, grid, map[map_row][map_col], grid_elem, tileset, trans_color, solid_threshold);
			}
			grid_col += GRID_BLOCK_COLUMNS;
		}
		std::cout << "Row " << map_row << " completed\n";
		grid_row += GRID_BLOCK_ROWS;
	}
	al_destroy_bitmap(tile_elem);
	al_destroy_bitmap(grid_elem);
}

void ComputeTileGridBlocks() {
	UnitTest utest;
	utest.Load(); // to load map
	utest.Initialise();
	std::vector<std::vector<byte>> map = utest.getMapRef(); // this is a ref.
	std::vector<std::vector<byte>> grid;
	ALLEGRO_BITMAP* tileset = utest.getTileset();
	ALLEGRO_COLOR trans_color = al_get_pixel(tileset, 16, 80); 
	Init(map, grid);
	
	ComputeTileGridBlocks1(map, grid);
	WriteGrid(grid, GRID1_OUT);
	for (auto row : grid)
		row.clear();
	ComputeTileGridBlocks2(map, grid, tileset, trans_color, 8);
	WriteGrid(grid, GRID2_OUT);
}
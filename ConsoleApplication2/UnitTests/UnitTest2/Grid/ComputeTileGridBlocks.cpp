#include "ComputeTileGridBlocks.h"
#include "Level1.h"

#define MAX_WIDTH 1024 // tiles??
#define MAX_HEIGHT 256 // tiles??
// maybe get actual map size from level 1.

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ These are from Lecture 8: Slide 16. 

#define GRID_ELEMENT_WIDTH 4 // in pixels
#define GRID_ELEMENT_HEIGHT 4 // in pixels

#define GRID_BLOCK_COLUMNS (TILE_WIDTH / GRID_ELEMENT_WIDTH) // 16/4 = 4
#define GRID_BLOCK_ROWS (TILE_HEIGHT / GRID_ELEMENT_HEIGHT) // 16/4 = 4
#define GRID_ELEMENTS_PER_TILE (GRID_BLOCK_ROWS * GRID_BLOCK_COLUMNS) // <-- ??



// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ These are from Lecture 8: Slide 33.

#define GRID1_OUT "UnitTests\\UnitTest2\\Grid\\grid1.csv"
#define GRID2_OUT "UnitTests\\UnitTest2\\Grid\\grid2.csv"

uint GRID_MAX_HEIGHT = -1;
uint GRID_MAX_WIDTH = -1;

// Inits grid to the correct size and the 2 vars above.
static void Init(std::vector<std::vector<byte>> &map, std::vector<std::vector<byte>> &grid) {
	GRID_MAX_HEIGHT = (uint)map.size() * GRID_BLOCK_ROWS;
	GRID_MAX_WIDTH = (uint)map[0].size() * GRID_BLOCK_COLUMNS;
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


static void ComputeTileGridBlocks1(std::vector<std::vector<byte>> &grid) {
	// JUST A DEMO gia na dw ti vgazei sto grid1.csv
	uint i = 0;
	for (uint a = 0; a < GRID_MAX_HEIGHT; a++) {
		for (uint b = 0; b < GRID_MAX_WIDTH; b++) {
			grid[a][b] = i++;
		}
	}
}

static void ComputeTileGridBlocks2() {

}

void ComputeTileGridBlocks() {
	UnitTest utest;
	utest.Load(); // to load map
	std::vector<std::vector<byte>> map = utest.getMapRef(); // this is a ref.
	std::vector<std::vector<byte>> grid;
	Init(map, grid);


	ComputeTileGridBlocks1(grid);
	WriteGrid(grid, GRID1_OUT);
	/*ComputeTileGridBlocks2();
	WriteGrid(grid, GRID2_OUT);*/
}
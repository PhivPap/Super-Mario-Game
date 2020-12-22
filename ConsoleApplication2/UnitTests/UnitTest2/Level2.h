#pragma once
#include "Level1.h"
#include <allegro5/allegro_primitives.h>

#define GRIDMAP_PATH "UnitTests//UnitTest2//Grid//grid1.csv"

class UnitTest2 : protected UnitTest {
	ALLEGRO_COLOR color;
	Rect rectangle{224, 112, 16, 16 };
	std::vector<std::vector<byte>> grid;
	

	std::function<void(void)> render_rect;


	void ReadTextGrid(std::vector<std::vector<byte>>&, Dim&);

public:
	UnitTest2();
	virtual void Initialise(void);
	virtual void Load(void);
	virtual void Clear(void);
	void Main();
};
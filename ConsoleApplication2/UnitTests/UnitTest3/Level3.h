#pragma once
#include "Level2.h"

class UnitTest3 : protected UnitTest2 {


	std::function<void(void)> mario_physics;


public:
	UnitTest3();
	virtual void Initialise(void);
	virtual void Load(void);
	virtual void Clear(void);
	void Main();
};
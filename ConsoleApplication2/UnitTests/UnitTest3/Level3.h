#pragma once
#include "Level2.h"

class UnitTest3 : protected UnitTest2 {



public:
	UnitTest3();
	virtual void Initialise(void);
	virtual void Load(void);
	virtual void Clear(void);
	void Main();
};
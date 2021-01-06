#pragma once
#include "Types.h"
#include <chrono>

class SystemClock final {
private:
	std::chrono::high_resolution_clock clock;
	static SystemClock singleton;

public:
	static SystemClock& Get(void);
	llu milli_secs(void)const;
	llu micro_secs(void)const;
	llu nano_secs(void)const;
};

static inline llu GetSystemTime (void) {
	return SystemClock::Get().milli_secs();
}
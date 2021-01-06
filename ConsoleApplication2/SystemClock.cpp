#include "SystemClock.h"

using namespace std::chrono;

SystemClock SystemClock::singleton;

llu SystemClock::milli_secs (void) const {
	return duration_cast<milliseconds>(clock.now().time_since_epoch()).count();
}

llu SystemClock::micro_secs (void) const {
	return duration_cast<microseconds>(clock.now().time_since_epoch()).count();
}

llu SystemClock::nano_secs(void) const {
	 return duration_cast<nanoseconds>(clock.now().time_since_epoch()).count();
}

SystemClock& SystemClock::Get(void) {
	return singleton;
}
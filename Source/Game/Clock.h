#ifndef CLOCK_H_INCLUDED
#define CLOCK_H_INCLUDED

#include <chrono>

namespace Meta {

class Clock {
public:
	Clock();
	double GetTimeNow();

private:
	std::chrono::high_resolution_clock::time_point _inceptionTime;
};

} // end of namespace Meta

#endif // CLOCK_H_INCLUDED
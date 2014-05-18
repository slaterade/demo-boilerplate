#include "Clock.h"

using namespace Meta;
using namespace std;

Clock::Clock()
	: _inceptionTime( chrono::high_resolution_clock::now() ) {
}

double Clock::GetTimeNow() {
	typedef chrono::duration<double, std::ratio<1>> SecondsFloatingPoint;
	auto timeNow = chrono::high_resolution_clock::now() - _inceptionTime;
	return chrono::duration_cast<SecondsFloatingPoint>( timeNow ).count();
}
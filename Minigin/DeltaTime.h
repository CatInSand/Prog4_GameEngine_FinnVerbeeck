#ifndef DELTA_TIME_H
#define DELTA_TIME_H

namespace dae
{
	static const int gFrameRate{ 60 };
	static const float gFixedDeltaTime{ 1.f/gFrameRate };
	static const int gMillisecondsPerFrame{ static_cast<int>(gFixedDeltaTime * 1000) };
	extern float gDeltaTime;
}

#endif

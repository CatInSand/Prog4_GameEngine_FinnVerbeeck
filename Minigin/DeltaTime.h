#ifndef DELTA_TIME_H
#define DELTA_TIME_H

namespace dae
{
	static int FRAME_RATE{ 60 };
	static float FIXED_DELTA_TIME{ 1.f/FRAME_RATE };
	static int MILLISECONDS_PER_FRAME{ static_cast<int>(FIXED_DELTA_TIME * 1000) };
	extern float gDeltaTime;
}

#endif

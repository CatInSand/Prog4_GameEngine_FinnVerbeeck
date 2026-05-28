#include "DeltaTime.h"

#include <cstdint>

#include <SDL3/SDL_timer.h>

float dae::time::gDeltaTime{ dae::time::FIXED_DELTA_TIME };

uint64_t& LastTimeSingle()
{
	static uint64_t lastTime{ SDL_GetTicks() };
	return lastTime;
}

void dae::time::CalculateDeltaTime()
{
	uint64_t currentTime{ SDL_GetTicks() };
	float deltaTime{ static_cast<float>((currentTime - LastTimeSingle()) / 1000.f) };

	LastTimeSingle() = currentTime;

	dae::time::gDeltaTime = deltaTime;
}
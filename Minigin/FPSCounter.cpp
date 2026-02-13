#include "FPSCounter.h"

void dae::FPSCounter::Update()
{
	m_FPS = 1.f / gDeltaTime;
}

float dae::FPSCounter::GetFPS() const
{
	return m_FPS;
}
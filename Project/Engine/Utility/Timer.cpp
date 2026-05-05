#include "Timer.h"
#include <cassert>
#include "DeltaTime.h"

dae::Timer::Timer(float duration)
	: m_Duration{ duration }
{
	assert(duration > 0.f);
}

void dae::Timer::Update()
{
	m_ElapsedTime += gDeltaTime;
}

void dae::Timer::Reset()
{
	m_ElapsedTime = 0.f;
}

float dae::Timer::ElapsedTime() const
{
	return m_ElapsedTime;
}
float dae::Timer::Duration() const
{
	return m_Duration;
}
bool dae::Timer::Done() const
{
	return m_ElapsedTime >= m_Duration;
}
bool dae::Timer::DoneThisFrame() const
{
	return m_ElapsedTime >= m_Duration && m_ElapsedTime - gDeltaTime < m_Duration;
}
#include "Timer.h"

#include <cassert>
#include <cmath>

#include "DeltaTime.h"

dae::Timer::Timer(float duration)
	: m_Duration{ duration }
{
	assert(duration > 0.f);
}

void dae::Timer::Update()
{
	m_ElapsedTime += time::gDeltaTime;
}
void dae::Timer::Reset()
{
	m_ElapsedTime = 0.f;
}
int dae::Timer::ModuloGet()
{
	int timesDone{ static_cast<int>(m_ElapsedTime / m_Duration) };
	m_ElapsedTime = std::fmod(m_ElapsedTime, m_Duration);;
	return timesDone;
}
bool dae::Timer::DecrementGet()
{
	if (Done())
	{
		m_ElapsedTime -= m_Duration;
		return true;
	}

	return false;
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
	return m_ElapsedTime >= m_Duration && m_ElapsedTime - time::gDeltaTime < m_Duration;
}
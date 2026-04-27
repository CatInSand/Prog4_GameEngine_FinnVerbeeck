#include "FPSCounter.h"
#include "GameObject.h"
#include <numeric>

dae::FPSCounter::FPSCounter(dae::GameObject* owner)
	: Component(owner)
{

}

void dae::FPSCounter::Update()
{
	m_Framerates.push_back(1.f / gDeltaTime);

	if (m_Framerates.size() == m_FrameCount)
	{
		float sum{ std::accumulate(m_Framerates.begin(), m_Framerates.end(), 0.f) };
		m_FPS = sum / m_FrameCount;

		m_Framerates.clear();
	}
}

float dae::FPSCounter::GetFPS() const
{
	return m_FPS;
}
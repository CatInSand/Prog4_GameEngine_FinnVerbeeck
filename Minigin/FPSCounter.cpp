#include "FPSCounter.h"
#include "GameObject.h"
#include <format>
#include <numeric>

dae::FPSCounter::FPSCounter(dae::GameObject* owner, const std::string& text, std::shared_ptr<dae::Font> font, const SDL_Color& color)
	: Text(owner, text, font, color)
{

}

void dae::FPSCounter::Update()
{
	m_Framerates.push_back(1.f / gDeltaTime);

	if (m_Framerates.size() == m_FrameCount)
	{
		float sum{ std::accumulate(m_Framerates.begin(), m_Framerates.end(), 0.f) };
		m_FPS = sum / m_FrameCount;

		const std::string fpsString{ std::format("{:.1f}", m_FPS) };
		if (fpsString != m_Text)
		{
			dae::Text::SetText(fpsString);
		}

		dae::Text::Update();

		m_Framerates.clear();
	}
}

float dae::FPSCounter::GetFPS() const
{
	return m_FPS;
}
#include "FPSCounter.h"
#include "GameObject.h"
#include <format>

dae::FPSCounter::FPSCounter(std::unique_ptr<dae::GameObject>& owner, const std::string& text, std::shared_ptr<dae::Font> font, const SDL_Color& color)
	: Text(owner, text, font, color)
{

}

void dae::FPSCounter::Update()
{
	m_FPS = 1.f / gDeltaTime;
}

float dae::FPSCounter::GetFPS() const
{
	return m_FPS;
}
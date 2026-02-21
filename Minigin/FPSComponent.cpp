#include "FPSComponent.h"
#include "FPSCounter.h"
#include "Text.h"
#include "GameObject.h"
#include <format>

dae::FPSComponent::FPSComponent(dae::GameObject* owner)
	: Component(owner)
{
	//Should be in Start function
	m_pFPSCounterComponent = Component::GetOwner()->GetComponent<dae::FPSCounter>();
	m_pTextComponent = Component::GetOwner()->GetComponent<dae::Text>();
}

void dae::FPSComponent::Update()
{
	const float framerate{ m_pFPSCounterComponent->GetFPS() };

	const std::string fpsString{ std::format("{:.1f}", framerate) };
	m_pTextComponent->SetText(fpsString);
}
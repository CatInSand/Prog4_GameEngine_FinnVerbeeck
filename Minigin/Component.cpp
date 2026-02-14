#include "Component.h"
#include "GameObject.h"

const std::string dae::Component::m_ID{ "Component" };

dae::Component::Component(std::unique_ptr<dae::GameObject>& owner)
	: m_Owner{ owner }
{

}

void dae::Component::SetEnabled(bool enabled)
{
	m_Enabled = enabled;
}
void dae::Component::ToggleEnabled()
{
	m_Enabled = !m_Enabled;
}
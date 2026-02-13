#include "Component.h"

const std::string dae::Component::m_ID{ "Component" };

void dae::Component::SetEnabled(bool enabled)
{
	m_Enabled = enabled;
}
void dae::Component::ToggleEnabled()
{
	m_Enabled = !m_Enabled;
}
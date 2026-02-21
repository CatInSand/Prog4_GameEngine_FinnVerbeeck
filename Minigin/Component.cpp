#include "Component.h"
#include "GameObject.h"

dae::Component::Component(dae::GameObject* owner)
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

void dae::Component::Delete()
{
	m_MarkedForDeletion = true;
}

bool dae::Component::IsMarkedForDeletion() const
{
	return m_MarkedForDeletion;
}

dae::GameObject* dae::Component::GetOwner()
{
	return m_Owner;
}
const dae::GameObject* dae::Component::GetOwner() const
{
	return m_Owner;
}
#include "Component.h"
#include "GameObject.h"

<<<<<<< Updated upstream
const std::string dae::Component::m_ID{ "Component" };

dae::Component::Component(std::unique_ptr<dae::GameObject>& owner)
=======
dae::Component::Component(dae::GameObject* owner)
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
=======
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
>>>>>>> Stashed changes
}
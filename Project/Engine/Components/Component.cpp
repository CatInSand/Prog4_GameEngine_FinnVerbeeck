#include "Component.h"
#include "GameObject.h"

dae::Component::Component(dae::GameObject* pOwner)
	: m_pOwner{ pOwner }
{

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
	return m_pOwner;
}
const dae::GameObject* dae::Component::GetOwner() const
{
	return m_pOwner;
}
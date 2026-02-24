#include "RotatorComponent.h"
#include "GameObject.h"
#include "DeltaTime.h"

dae::RotatorComponent::RotatorComponent(GameObject* owner, const glm::vec3& center, float speed)
	: Component(owner)
	, m_Center{ center }
	, m_Speed{ speed }
{
	Initialize();
}

void dae::RotatorComponent::Update()
{
	m_CurrentAngle += m_Speed * dae::gDeltaTime;
	GetOwner()->SetLocalPosition(
		m_Center.x + cos(m_CurrentAngle) * m_Distance,
		m_Center.y + sin(m_CurrentAngle) * m_Distance
	);
}

void dae::RotatorComponent::Initialize()
{
	glm::vec3 centerToObject{ Component::GetOwner()->GetWorldTransform().GetPosition() - m_Center };
	m_CurrentAngle = atan2(centerToObject.y, centerToObject.x);
	m_Distance = glm::length(centerToObject);
}
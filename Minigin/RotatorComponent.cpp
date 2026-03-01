#include "RotatorComponent.h"
#include "GameObject.h"
#include "DeltaTime.h"

dae::RotatorComponent::RotatorComponent(GameObject* owner, float speed)
	: Component(owner)
	, m_Speed{ speed }
{
	Initialize();
}

void dae::RotatorComponent::Update()
{
	constexpr float TAU{ 2 * 3.1415f };

	m_CurrentAngle += m_Speed * dae::gDeltaTime;

	m_CurrentAngle = m_CurrentAngle - floor(m_CurrentAngle / TAU) * TAU;

	GetOwner()->SetLocalPosition(
		cos(m_CurrentAngle) * m_Distance,
		sin(m_CurrentAngle) * m_Distance
	);
}

void dae::RotatorComponent::Initialize()
{
	glm::vec3 centerToObject{ Component::GetOwner()->GetWorldTransform().GetPosition() };
	m_CurrentAngle = atan2(centerToObject.y, centerToObject.x);
	m_Distance = glm::length(centerToObject);
}
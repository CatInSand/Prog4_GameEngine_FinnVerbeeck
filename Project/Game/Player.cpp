#include "Player.h"
#include "EventQueue.h"
#include "DeltaTime.h"
#include "Events.h"
#include "Tags.h"
#include "Shoot.h"


dae::PlayerComponent::PlayerComponent(GameObject* pOwner)
	: ColliderComponent(pOwner, pOwner->LocalTransformPtr())
	, m_pIdleState{ std::make_unique<IdleState>(this) }
	, m_pCurrentState{ m_pIdleState.get() }
{
	m_pGridComponent = SceneManager::Instance().CurrentScene()->GetObjectWithName("Grid"_h)->GetComponent<GridComponent>();
	SetStartPosition();

	//make pump
	std::unique_ptr<GameObject> gameObject{ std::make_unique<GameObject>(GetOwner(), "Pump") };

	std::unique_ptr<PumpComponent> pumpComponent{ std::make_unique<PumpComponent>(gameObject.get()) };
	m_pPumpComponent = pumpComponent.get();
	gameObject->AddComponent<PumpComponent>(std::move(pumpComponent));

	SceneManager::Instance().CurrentScene()->Add(std::move(gameObject));
}

void dae::PlayerComponent::Update()
{
	m_pCurrentState = m_pCurrentState->Update();
}

void dae::PlayerComponent::Move(Direction direction)
{
	if (m_pGridComponent->CanDigInDir(GetOwner()->GetLocalTransform().position, direction, m_Direction))
	{
		m_PreviousDirection = m_Direction;
		m_Direction = direction;

		GetOwner()->SetLocalPosition(
			m_pGridComponent->DigInDir(
				GetOwner()->GetLocalTransform().position,
				m_Direction,
				m_PreviousDirection,
				m_MoveSpeed * time::gDeltaTime
			)
		);
	}
}
void dae::PlayerComponent::Shoot(Direction)
{
	
}

void dae::PlayerComponent::OnCollisionEnter(GameObject* pObject)
{
	if (pObject->HasTag(TAG_DANGEROUS_TO_PLAYER))
	{
		--m_LiveCount;
		if (m_LiveCount == 0)
		{
			EventQueue::Instance().Enqueue(std::make_unique<EventGameOver>());
		}
		else
		{
			SetStartPosition();
			EventQueue::Instance().Enqueue(std::make_unique<EventPlayerDied>());
		}
	}
};

void dae::PlayerComponent::SetStartPosition()
{
	GetOwner()->SetLocalPosition(m_pGridComponent->PlayerSpawn());
}
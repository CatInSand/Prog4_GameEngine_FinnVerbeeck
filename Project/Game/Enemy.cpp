#include "Enemy.h"
#include "EventQueue.h"
#include "SceneManager.h"
#include "DeltaTime.h"

dae::EnemyComponent::EnemyComponent(GameObject* pOwner, bool isPooka, EnemyState* pStartingState)
	: ColliderComponent(pOwner, pOwner->LocalTransformPtr())
	, m_pCurrentState{ pStartingState }
	, m_IsPooka{ isPooka }
{
	m_pPlayer = SceneManager::Instance().CurrentScene()->GetObjectWithName("Player"_h);
	m_pGridComponent = SceneManager::Instance().CurrentScene()->GetObjectWithName("Grid"_h)->GetComponent<GridComponent>();
	if (m_pCurrentState != nullptr)
	{
		m_pCurrentState->Enter();
	}
}

void dae::EnemyComponent::Update()
{
	EnemyState* newState{ m_pCurrentState->Update() };
	if (newState != m_pCurrentState)
	{
		m_pCurrentState->Exit();
		m_pCurrentState = newState;
		m_pCurrentState->Enter();
	}
}
dae::GameObject* dae::EnemyComponent::Owner()
{
	return GetOwner();
}
void dae::EnemyComponent::Move(const glm::vec2 movement)
{
	GetOwner()->SetLocalPosition(GetOwner()->GetLocalTransform().position + movement * SPEED * time::gDeltaTime);
}

void dae::EnemyComponent::OnCollisionEnter(GameObject* pObject)
{
	if (pObject->HasTag(TAG_DANGEROUS_TO_ENEMY))
	{
		EventQueue::Instance().Enqueue(std::make_unique<EventEnemyDied>(m_IsPooka));
		GetOwner()->Delete();
	}
}

// Pooka

dae::PookaComponent::PookaComponent(GameObject* pOwner)
	: EnemyComponent(pOwner, true, nullptr)
	, m_pIdleState{ std::make_unique<IdleState>(this) }
	, m_pChaseState{ std::make_unique<ChaseState>(this) }
{
	m_pCurrentState = m_pIdleState.get();
	m_pCurrentState->Enter();
}

dae::PookaComponent::IdleState::IdleState(EnemyComponent* pData)
	: EnemyState(pData)
{
}
dae::PookaComponent::EnemyState* dae::PookaComponent::IdleState::Update()
{
	switch (m_pEnemyComponent->m_Direction)
	{
	case Direction::none:
		break;
	case Direction::up:
		m_pEnemyComponent->Move({ 0.f, -1.f });
		break;
	case Direction::down:
		m_pEnemyComponent->Move({ 0.f, 1.f });
		break;
	case Direction::left:
		m_pEnemyComponent->Move({ -1.f, 0.f });
		break;
	case Direction::right:
		m_pEnemyComponent->Move({ 1.f, 0.f });
		break;
	default:
		break;
	}

	float distance{ glm::distance(m_pEnemyComponent->Owner()->GetLocalTransform().position, m_pEnemyComponent->m_pPlayer->GetLocalTransform().position) };

	if (distance < m_pEnemyComponent->m_PlayerDetectionRadius)
	{
		return dynamic_cast<PookaComponent*>(m_pEnemyComponent)->m_pChaseState.get();
	}

	return this;
}
void dae::PookaComponent::IdleState::Enter()
{
	for (Direction direction{}; direction < Direction::count; direction = static_cast<Direction>(static_cast<int>(direction) + 1))
	{
		if (m_pEnemyComponent->m_pGridComponent->CanMoveInDir(m_pEnemyComponent->Owner()->GetLocalTransform().position, Direction::down))
		{
			m_pEnemyComponent->m_Direction = direction;
		}
	}

	m_pEnemyComponent->m_Direction = Direction::right;
}
void dae::PookaComponent::IdleState::Exit()
{
}

dae::PookaComponent::ChaseState::ChaseState(EnemyComponent* pData)
	: EnemyState(pData)
{
}
dae::PookaComponent::EnemyState* dae::PookaComponent::ChaseState::Update()
{
	m_pEnemyComponent->Move(glm::normalize(m_pEnemyComponent->m_pPlayer->GetLocalTransform().position - m_pEnemyComponent->Owner()->GetLocalTransform().position));
	return this;
}
void dae::PookaComponent::ChaseState::Enter()
{

}
void dae::PookaComponent::ChaseState::Exit()
{
}
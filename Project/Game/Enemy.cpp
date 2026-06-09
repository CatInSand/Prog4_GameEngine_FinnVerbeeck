#include "Enemy.h"
#include "EventQueue.h"
#include "SceneManager.h"
#include "DeltaTime.h"

dae::EnemyComponent::EnemyComponent(GameObject* pOwner, bool isPooka, EnemyState* pStartingState)
	: ColliderComponent(pOwner, pOwner->LocalTransformPtr())
	, m_pCurrentState{ pStartingState }
	, m_IsPooka{ isPooka }
{
	pOwner->AddTag(TAG_DANGEROUS_TO_PLAYER);
}

void dae::EnemyComponent::Start()
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
void dae::EnemyComponent::Move(Direction direction)
{
	m_Direction = direction;

	if (m_pGridComponent->CanMoveInDir(GetOwner()->GetLocalTransform().position, direction))
	{
		GetOwner()->SetLocalPosition(
			m_pGridComponent->MoveInDir(
				GetOwner()->GetLocalTransform().position,
				direction,
				SPEED * time::gDeltaTime
			)
		);
	}
}
void dae::EnemyComponent::MovePhase(const glm::vec2 direction)
{
	GetOwner()->SetLocalPosition(
		GetOwner()->GetLocalTransform().position
		+ glm::normalize(direction) * SPEED * time::gDeltaTime
	);
}

void dae::EnemyComponent::OnCollisionEnter(GameObject* pObject)
{
	if (pObject->HasTag(TAG_DANGEROUS_TO_ENEMY))
	{
		int layer{ m_pGridComponent->GetLayer(GetOwner()->GetLocalTransform().position) };
		EventQueue::Instance().Enqueue(std::make_unique<EventEnemyDied>(layer, m_IsPooka));
		GetOwner()->Delete();
	}
}

// Pooka

dae::PookaComponent::PookaComponent(GameObject* pOwner)
	: EnemyComponent(pOwner, true, nullptr)
	, m_pIdleState{ std::make_unique<IdleState>(this) }
	, m_pChaseState{ std::make_unique<ChaseState>(this) }
{
}

void dae::PookaComponent::Start()
{
	EnemyComponent::Start();
	m_pCurrentState = m_pIdleState.get();
	m_pCurrentState->Enter();
}

dae::EnemyComponent::EnemyState::EnemyState(EnemyComponent* pData)
	: m_pEnemyComponent{ pData }
{
}

dae::Direction dae::EnemyComponent::EnemyState::GetNewDirection(Direction currentDir)
{
	for (Direction direction{}; direction < Direction::count; direction = static_cast<Direction>(static_cast<int>(direction) + 1))
	{
		if (m_pEnemyComponent->m_pGridComponent->CanMoveInDir(
				m_pEnemyComponent->Owner()->GetLocalTransform().position, direction))
		{
			return direction;
		}
	}

	// return static_cast<Direction>((static_cast<int>(currentDir) + 1) % static_cast<int>(Direction::count));
	return currentDir;
}

dae::PookaComponent::IdleState::IdleState(EnemyComponent* pData)
	: EnemyState(pData)
{
}
dae::PookaComponent::EnemyState* dae::PookaComponent::IdleState::Update()
{
	glm::vec2 oldPos{ m_pEnemyComponent->Owner()->GetLocalTransform().position };
	m_pEnemyComponent->Move(m_pEnemyComponent->m_Direction);
	if (oldPos == m_pEnemyComponent->Owner()->GetLocalTransform().position)
	{
		m_pEnemyComponent->m_Direction = GetNewDirection(m_pEnemyComponent->m_Direction);
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
	m_pEnemyComponent->m_Direction = GetNewDirection(Direction::none);
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
	m_pEnemyComponent->MovePhase(m_pEnemyComponent->m_pPlayer->GetLocalTransform().position - m_pEnemyComponent->Owner()->GetLocalTransform().position);
	return this;
}
void dae::PookaComponent::ChaseState::Enter()
{

}
void dae::PookaComponent::ChaseState::Exit()
{
}
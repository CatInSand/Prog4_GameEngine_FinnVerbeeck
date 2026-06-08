#include "Player.h"
#include "EventQueue.h"
#include "DeltaTime.h"
#include "Events.h"
#include "Tags.h"
#include "GameSounds.h"


dae::PlayerComponent::PlayerComponent(GameObject* pOwner)
	: ColliderComponent(pOwner, pOwner->LocalTransformPtr())
	, m_pIdleState{ std::make_unique<IdleState>(this) }
	, m_pCurrentState{ m_pIdleState.get() }
{
	m_pGridComponent = SceneManager::Instance().CurrentScene()->GetObjectWithName("Grid"_h)->GetComponent<GridComponent>();
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

void dae::PlayerComponent::OnCollisionEnter(GameObject* pObject)
{
	if (pObject->HasTag(TAG_DANGEROUS_TO_PLAYER))
	{
		--m_LiveCount;
		if (m_LiveCount == 0)
		{
			ServiceLocator::GetSoundSystem().StopAll();
			ServiceLocator::GetSoundSystem().Play(static_cast<sound_id>(Sounds::game_over), 1.f);
			EventQueue::Instance().Enqueue(std::make_unique<EventPlayerDied>());
		}
	}
};
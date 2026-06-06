#ifndef PLAYER_H
#define PLAYER_H

#include "ColliderComponent.h"
#include "Component.h"
#include "StateMachine.h"
#include "EventQueue.h"
#include "DeltaTime.h"
#include "BaseCommand.h"

namespace dae
{
	class EventPlayerDied final : public Event
	{
	public:
		EventPlayerDied()
			: Event("EventPlayerDied"_h)
		{}
	};

	class PlayerComponent final : public ColliderComponent
	{
		enum class Direction {
			none = -1,
			up,
			forward,
			down,
		};

	public:
		PlayerComponent(GameObject* pOwner)
			: ColliderComponent(pOwner, pOwner->LocalTransformPtr())
			, m_pIdleState{ std::make_unique<IdleState>(this) }
			, m_pCurrentState{ m_pIdleState.get() }
		{
		}

		virtual void Update() override
		{
			m_pCurrentState = m_pCurrentState->Update();
		}

		void Move(const glm::vec2& direction)
		{
			GetOwner()->SetLocalPosition(GetOwner()->GetLocalTransform().position + direction * m_MoveSpeed * time::gDeltaTime);

			if (direction.x != 0)
				m_Direction = Direction::forward;
			else if (direction.y > 0)
				m_Direction = Direction::up;
			else if (direction.y < 0)
				m_Direction = Direction::down;

			if (direction.x > 0)
				m_Right = true;
			else if (direction.x < 0)
				m_Right = false;
		}

		int m_LiveCount{ 6 };
		float m_MoveSpeed{ 100.f };

	private:
		Direction m_Direction{ Direction::forward };
		bool m_Right{ true };

		virtual void OnCollisionEnter(GameObject* pObject) {
			if (pObject->HasTag("Enemy"_h))
			{
				--m_LiveCount;
				if (m_LiveCount == 0)
				{
					EventQueue::Instance().Enqueue(std::make_unique<EventPlayerDied>());
				}
			}
		};
		virtual void OnCollisionStay(GameObject*) {};
		virtual void OnCollisionExit(GameObject*) {};

		class State {
		public:
			State(PlayerComponent* player)
				: m_pPlayerComponent{ player }
			{}
			virtual ~State() = default;

			virtual State* Update() = 0;

			PlayerComponent* m_pPlayerComponent;
		};
		class IdleState : public State {
		public:
			IdleState(PlayerComponent* player)
				: State(player)
			{}
			virtual ~IdleState() = default;
			virtual State* Update()
			{
				return this;
			}

			virtual void Enter()
			{

			}
			virtual void Exit()
			{

			}
		};

		std::unique_ptr<IdleState> m_pIdleState;
		State* m_pCurrentState;
	};

	class PlayerMoveCommand final : public BaseCommand
	{
	public:
		PlayerMoveCommand(PlayerComponent* pPlayer, const glm::vec2& direction)
			: m_Direction{ direction }
			, m_pPlayer{ pPlayer }
		{}
		virtual ~PlayerMoveCommand() = default;
		virtual void Execute()
		{
			m_pPlayer->Move(m_Direction);
		}

	private:
		glm::vec2 m_Direction;
		PlayerComponent* m_pPlayer;
	};
}

#endif

#ifndef PLAYER_H
#define PLAYER_H

#include "ColliderComponent.h"
#include "StateMachine.h"
#include "BaseCommand.h"
#include "Grid.h"
#include "Shoot.h"

namespace dae
{
	class GridComponent;
	class PlayerComponent final : public ColliderComponent
	{
	public:
		PlayerComponent(GameObject* pOwner);

		virtual void Update() override;
		void Move(Direction direction);
		void Shoot();

		int m_LiveCount{ 4 };
		float m_MoveSpeed{ 25.f };

		GridComponent* m_pGridComponent{ nullptr };

	private:
		Direction m_Direction{ Direction::right };
		Direction m_PreviousDirection{ Direction::right };
		bool m_Right{ true };
		PumpComponent* m_pPumpComponent{ nullptr };

		virtual void OnCollisionEnter(GameObject* pObject) override;
		virtual void OnCollisionStay(GameObject*) override {};
		virtual void OnCollisionExit(GameObject*) override {};

		void SetStartPosition();

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
		PlayerMoveCommand(PlayerComponent* pPlayer, Direction direction)
			: m_Direction{ direction }
			, m_pPlayer{ pPlayer }
		{}
		virtual ~PlayerMoveCommand() = default;
		virtual void Execute()
		{
			m_pPlayer->Move(m_Direction);
		}

	private:
		Direction m_Direction;
		PlayerComponent* m_pPlayer;
	};

	class PlayerShootCommand final : public BaseCommand
	{
	public:
		PlayerShootCommand(PlayerComponent* pPlayer)
			: m_pPlayer{ pPlayer }
		{}
		virtual ~PlayerShootCommand() = default;
		virtual void Execute()
		{
			m_pPlayer->Shoot();
		}

	private:
		PlayerComponent* m_pPlayer;
	};
}

#endif

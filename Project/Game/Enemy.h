#ifndef ENEMY_H
#define ENEMY_H

#include "ColliderComponent.h"
#include "Tags.h"
#include "Events.h"
#include "StateMachine.h"
#include "Grid.h"

namespace dae
{
	class EnemyComponent : public ColliderComponent
	{
	public:
		virtual ~EnemyComponent() = default;

		virtual void Start() override;
		virtual void Update() override;
		void Move(Direction direction);
		void MovePhase(const glm::vec2 direction);
		GameObject* Owner();

		Direction m_Direction{ Direction::none };

		float m_PlayerDetectionRadius{ 50.f };

		GridComponent* m_pGridComponent{ nullptr };
		GameObject* m_pPlayer{ nullptr };

	protected:
		static constexpr float SPEED{ 10.f };

		class EnemyState
		{
		public:
			virtual ~EnemyState() = default;
			virtual EnemyState* Update() = 0;

			virtual void Enter() = 0;
			virtual void Exit() = 0;

		protected:
			explicit EnemyState(EnemyComponent* pData);

			Direction GetNewDirection(Direction currentDir);

			EnemyComponent* m_pEnemyComponent;
		};

		explicit EnemyComponent(GameObject* pOwner, bool isPooka, EnemyState* pStartingState);

		EnemyState* m_pCurrentState{ nullptr };

	private:
		bool m_IsPooka;

		virtual void OnCollisionEnter(GameObject* pObject) override;
		virtual void OnCollisionStay(GameObject*) override {}
		virtual void OnCollisionExit(GameObject*) override {}
	};

	class PookaComponent final : public EnemyComponent
	{
	public:
		PookaComponent(GameObject* pOwner);
		virtual ~PookaComponent() = default;

		virtual void Start() override;

	private:
		class IdleState final : public EnemyState
		{
		public:
			IdleState(EnemyComponent* pData);
			virtual ~IdleState() = default;
			virtual EnemyState* Update() override;

			virtual void Enter() override;
			virtual void Exit() override;
		};

		class ChaseState final : public EnemyState
		{
		public:
			ChaseState(EnemyComponent* pData);
			virtual ~ChaseState() = default;
			virtual EnemyState* Update() override;

			virtual void Enter() override;
			virtual void Exit() override;
		};

		std::unique_ptr<IdleState> m_pIdleState;
		std::unique_ptr<ChaseState> m_pChaseState;
	};
}

#endif

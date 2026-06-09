#ifndef SHOOT_H
#define SHOOT_H

#include "ColliderComponent.h"

namespace dae
{
	class PumpComponent final : public ColliderComponent
	{
	public:
		PumpComponent(GameObject* pOwner)
			: ColliderComponent(pOwner, pOwner->LocalTransformPtr())
		{
		}
		virtual ~PumpComponent() = default;

		virtual void Update() override
		{
			if (m_ShootingStage == ShootingStage::shoot)
			{

			}
			else if (m_ShootingStage == ShootingStage::pump)
			{

			}
		}

		void Shoot(Direction direction)
		{
			m_Direction = direction;
			m_ShootingStage = ShootingStage::shoot;
		}
		void StopShooting()
		{
			m_ShootingStage = ShootingStage::none;
		}

	private:
		virtual void OnCollisionEnter(GameObject*) override {};
		virtual void OnCollisionStay(GameObject*) override {};
		virtual void OnCollisionExit(GameObject*) override {};

		enum class ShootingStage {
			none,
			shoot,
			pump,
		};

		ShootingStage m_ShootingStage{ ShootingStage::none };
		Direction m_Direction{ Direction::none };
	};
}

#endif

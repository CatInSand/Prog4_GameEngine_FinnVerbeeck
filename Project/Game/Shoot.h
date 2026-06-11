#ifndef SHOOT_H
#define SHOOT_H

#include "ColliderComponent.h"
#include "TextureComponent.h"
#include "DeltaTime.h"
#include "Tags.h"

namespace dae
{
	class PumpComponent final : public ColliderComponent
	{
	public:
		PumpComponent(GameObject* pOwner)
			: ColliderComponent(pOwner, pOwner->LocalTransformPtr())
		{
			GetOwner()->AddTag(TAG_DANGEROUS_TO_ENEMY);

			auto textureComponent{ std::make_unique<TextureComponent>(GetOwner()) };
			m_pTextureComponent = textureComponent.get();
			m_pTextureComponent->m_Enabled = false;
			GetOwner()->AddComponent<TextureComponent>(std::move(textureComponent));
		}
		virtual ~PumpComponent() = default;

		virtual void Update() override
		{
			if (m_ShootingStage == ShootingStage::shoot)
			{
				m_Distance += m_MoveSpeed * time::gDeltaTime;
				if (m_Distance >= END_DISTANCE)
				{
					m_Distance = END_DISTANCE;
					m_ShootingStage = ShootingStage::pump;
				}

				glm::vec2 deltaPos{};
				switch (m_Direction)
				{
				case dae::Direction::up:
					deltaPos = { 0.f, -1.f };
					break;
				case dae::Direction::down:
					deltaPos = { 0.f, 1.f };
					break;
				case dae::Direction::left:
					deltaPos = { -1.f, 0.f };
					break;
				case dae::Direction::right:
					deltaPos = { 1.f, 0.f };
					break;
				default:
					break;
				}
				deltaPos *= m_MoveSpeed * time::gDeltaTime;
				GetOwner()->SetLocalPosition(GetOwner()->GetLocalTransform().position + deltaPos);
			}
			else if (m_ShootingStage == ShootingStage::pump)
			{
				m_pTextureComponent->m_Enabled = false;
			}
		}

		void Shoot(Direction direction)
		{
			if (!m_pTextureComponent->m_Enabled)
			{
				m_pTextureComponent->m_Enabled = true;
				GetOwner()->SetLocalPosition({});
				m_ShootingStage = ShootingStage::shoot;
				m_Distance = START_DISTANCE;

				if (m_Direction != direction)
				{
					m_Direction = direction;
					switch (m_Direction)
					{
					case dae::Direction::up:
						m_pTextureComponent->SetTexture("sprites/hose_up.png");
						GetOwner()->SetLocalScale(m_pTextureComponent->GetSize());
						break;
					case dae::Direction::down:
						m_pTextureComponent->SetTexture("sprites/hose_down.png");
						GetOwner()->SetLocalScale(m_pTextureComponent->GetSize());
						break;
					case dae::Direction::left:
						m_pTextureComponent->SetTexture("sprites/hose_left.png");
						GetOwner()->SetLocalScale(m_pTextureComponent->GetSize());
						break;
					case dae::Direction::right:
						m_pTextureComponent->SetTexture("sprites/hose_right.png");
						GetOwner()->SetLocalScale(m_pTextureComponent->GetSize());
						break;
					default:
						break;
					}
				}
			}
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

		static constexpr float m_MoveSpeed{ 16.f };
		static constexpr float START_DISTANCE{ 0.f };
		static constexpr float END_DISTANCE{ 16.f };
		float m_Distance{ START_DISTANCE };

		TextureComponent* m_pTextureComponent{ nullptr };
	};
}

#endif

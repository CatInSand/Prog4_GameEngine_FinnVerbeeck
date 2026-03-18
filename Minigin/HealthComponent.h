#ifndef HEALTH_COMPONENT_H
#define HEALTH_COMPONENT_H

#include "Component.h"
#include "Subject.h"

namespace dae
{
	class HealthComponent final : public Component
	{
	public:
		HealthComponent(GameObject* pOwner, unsigned int maxHealth, Observer* pObserver = nullptr);
		virtual ~HealthComponent() = default;
		HealthComponent(const HealthComponent& other) = delete;
		HealthComponent(HealthComponent&& other) = delete;
		HealthComponent& operator=(const HealthComponent& other) = delete;
		HealthComponent& operator=(HealthComponent&& other) = delete;

		virtual void Update() override;

		void Damage(int damage);
		bool IsDead() const;
		int Health() const;

	private:
		const unsigned int m_MaxHealth;
		int m_CurrentHealth;
		Subject m_Subject{};
	};
}

#endif
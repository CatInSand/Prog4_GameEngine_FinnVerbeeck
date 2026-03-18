#include "HealthComponent.h"
#include "GameObject.h"

dae::HealthComponent::HealthComponent(dae::GameObject* pOwner, unsigned int maxHealth, Observer* pObserver)
	: Component(pOwner)
	, m_MaxHealth{ maxHealth }
	, m_CurrentHealth{ static_cast<int>(maxHealth) }
{
	if (pObserver != nullptr)
	{
		m_Subject.AddObserver(pObserver);
	}
}

void dae::HealthComponent::Update()
{
}

void dae::HealthComponent::Damage(int damage)
{
	if (m_CurrentHealth > 0 && m_CurrentHealth - damage <= 0)
	{
		std::unique_ptr<dae::Event> event{ std::make_unique<dae::EventPlayerDied>(GetOwner()) };
		m_Subject.NotifyObservers(event);
	}

	m_CurrentHealth -= damage;
}
bool dae::HealthComponent::IsDead() const
{
	return m_CurrentHealth < 0;
}
int dae::HealthComponent::Health() const
{
	return m_CurrentHealth;
}
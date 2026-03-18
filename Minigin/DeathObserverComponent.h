#ifndef DEATH_OBSERVER_COMPONENT_H
#define DEATH_OBSERVER_COMPONENT_H

#include "Component.h"
#include "Observer.h"

namespace dae
{
	class GameObject;
	class DeathObserverComponent final : public Component
	{
	public:
		DeathObserverComponent(GameObject* pOwner)
			: Component(pOwner)
		{
		}

		virtual void Update() override {}

		DeathObserver* Get() const { return m_pDeathObserver.get(); }

	private:
		std::unique_ptr<DeathObserver> m_pDeathObserver{ std::make_unique<DeathObserver>() };
	};
}

#endif
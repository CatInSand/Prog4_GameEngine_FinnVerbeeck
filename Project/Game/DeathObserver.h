#ifndef DEATH_OBSERVER_H
#define DEATH_OBSERVER_H

#include "Observer.h"

namespace dae
{
	class DeathObserver final : public Observer
	{
	public:
		virtual ~DeathObserver() = default;
		virtual void Notify(std::unique_ptr<Event>& pEvent) override;
	};
}

#endif
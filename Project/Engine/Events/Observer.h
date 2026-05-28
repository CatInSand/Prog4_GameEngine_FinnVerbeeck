#ifndef OBSERVER_H
#define OBSERVER_H

#include "Event.h"
#include <memory>

namespace dae
{
	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(std::unique_ptr<Event>& pEvent) = 0;
	};
}

#endif
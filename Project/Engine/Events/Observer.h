#ifndef OBSERVER_H
#define OBSERVER_H

#include <memory>

#include "Event.h"

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
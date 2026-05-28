#ifndef GAME_EVENTS_H
#define GAME_EVENTS_H

#include "Event.h"

namespace dae
{
	class EventPlayerDied final : public Event
	{
	public:
		EventPlayerDied(GameObject* pTarget)
			: Event("PlayerDied"_h)
			, m_pTarget{ pTarget }
		{}
		GameObject* m_pTarget;
	};
}

#endif
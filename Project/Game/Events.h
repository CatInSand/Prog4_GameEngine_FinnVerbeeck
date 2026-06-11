#ifndef EVENTS_H
#define EVENTS_H

#include "Event.h"

namespace dae
{
	class EventPlayerDied final : public Event
	{
	public:
		EventPlayerDied()
			: Event("EventPlayerDied"_h)
		{}
		virtual ~EventPlayerDied() = default;
	};

	class EventGameOver final : public Event
	{
	public:
		EventGameOver()
			: Event("EventGameOver"_h)
		{}
		virtual ~EventGameOver() = default;
	};

	class EventEnemyDied final : public Event
	{
	public:
		EventEnemyDied(int layer, bool pooka)
			: Event("EventEnemyDied"_h)
			, m_Layer{ layer }
			, m_Pooka{ pooka }
		{}
		virtual ~EventEnemyDied() = default;

		int m_Layer;
		bool m_Pooka;
	};
}

#endif
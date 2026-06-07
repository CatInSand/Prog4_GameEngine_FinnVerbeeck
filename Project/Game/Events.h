#ifndef EVENTS_H
#define EVENTS_H

#include "Event.h"

namespace dae
{
	class EventEnemyDied final : public Event
	{
	public:
		EventEnemyDied(bool pooka_)
			: Event("EventEnemyDied"_h)
			, pooka{ pooka_ }
		{}

		bool pooka;
	};

	class EventPlayerDied final : public Event
	{
	public:
		EventPlayerDied()
			: Event("EventPlayerDied"_h)
		{}
	};

	class ScoreEvent final : public Event
	{
	private:
		static constexpr int m_RockScores[]{
			INT_MIN,
			1000,
			2500,
			4000,
			6000,
			8000,
			12000,
			15000
		};

	public:
		ScoreEvent(int layer, bool fygar)
			: Event("ScoreEvent"_h)
			, deltaScore{ fygar ? (100 * (1 + layer)) : (200 * (1 + layer)) }
		{
			assert(0 < layer && layer < 5);
		}
		ScoreEvent(int numberKilledByRock)
			: Event("ScoreEvent"_h)
			, deltaScore{ m_RockScores[numberKilledByRock] }
		{
			assert(0 < numberKilledByRock && numberKilledByRock < 9);
		}
		ScoreEvent(bool reset_)
			: Event("ScoreEvent"_h)
			, reset{ reset_ }
		{}

		int deltaScore{ 0 };
		bool reset{ false };
	};
}

#endif
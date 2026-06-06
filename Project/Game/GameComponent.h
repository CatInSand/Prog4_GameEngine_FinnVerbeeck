#ifndef GAME_COMPONENT_H
#define GAME_COMPONENT_H

#include "Component.h"
#include "Observer.h"
#include "EventQueue.h"
#include "TextComponent.h"

namespace dae
{
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
		{
		}

		int deltaScore{ 0 };
		bool reset{ false };
	};

	class GameComponent final : public Component, public Observer
	{
	public:
		GameComponent(GameObject* pOwner, TextComponent* pScoreText)
			: Component(pOwner)
			, m_pScoreText{ pScoreText }
		{
			EventQueue::Instance().AddObserver(this);
		}
		virtual ~GameComponent()
		{
			EventQueue::Instance().RemoveObserver(this);
		}
		GameComponent(const GameComponent& other) = delete;
		GameComponent(GameComponent&& other) = delete;
		GameComponent& operator=(const GameComponent& other) = delete;
		GameComponent& operator=(GameComponent&& other) = delete;

		virtual void Update() override
		{
			if (m_ScoreChanged)
			{
				m_ScoreChanged = false;
				m_pScoreText->SetText(std::format("SCORE {}", m_Score));
			}
		}
		virtual void Notify(std::unique_ptr<Event>& pEvent)
		{
			switch (pEvent->m_ID)
			{
			case "ScoreEvent"_h:
			{
				ScoreEvent* pCurrentEvent{ dynamic_cast<ScoreEvent*>(pEvent.get()) };
				if (pCurrentEvent->reset)
				{
					m_Score = 0;
				}
				else
				{
					m_Score += pCurrentEvent->deltaScore;
					assert(m_Score >= 0);
					m_ScoreChanged = true;
				}
				break;
			}
			default:
				break;
			}
		}

	private:
		int m_Score{};
		bool m_ScoreChanged{ true };

		TextComponent* m_pScoreText;
	};
}

#endif
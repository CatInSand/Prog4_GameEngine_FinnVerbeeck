#ifndef SCORE_COMPONENT_H
#define SCORE_COMPONENT_H

#include "Component.h"
#include "Observer.h"
#include "EventQueue.h"

namespace dae
{
	class ScoreComponent final : public Component, public Observer
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
		ScoreComponent(GameObject* pOwner, TextComponent* pScoreText)
			: Component(pOwner)
			, m_pScoreText{ pScoreText }
		{
			EventQueue::Instance().AddObserver(this);
		}
		virtual ~ScoreComponent()
		{
			EventQueue::Instance().RemoveObserver(this);
		}

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
			case "EventEnemyDied"_h:
			{
				EventEnemyDied* pCurrentEvent{ dynamic_cast<EventEnemyDied*>(pEvent.get()) };
				
				int deltaScore{
					pCurrentEvent->m_Pooka
					? (100 * (1 + pCurrentEvent->m_Layer))
					: (200 * (1 + pCurrentEvent->m_Layer)) 
				};

				m_Score += deltaScore;
				m_ScoreChanged = true;

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

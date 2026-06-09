#ifndef GAME_COMPONENT_H
#define GAME_COMPONENT_H

#include "Component.h"
#include "Observer.h"
#include "EventQueue.h"
#include "TextComponent.h"
#include "Events.h"
#include "GameSounds.h"
#include "Grid.h"

namespace dae
{
	class GameComponent final : public Component, public Observer
	{
	public:
		GameComponent(GameObject* pOwner, TextComponent* pScoreText)
			: Component(pOwner)
			, m_pScoreText{ pScoreText }
		{
			EventQueue::Instance().AddObserver(this);
			ServiceLocator::GetSoundSystem().Play(static_cast<sound_id>(Sounds::game_music), 1.f);
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
			case "EventPlayerDied"_h:
			{
				SceneManager::Instance().CurrentScene()->GetObjectWithName("Grid"_h)->GetComponent<GridComponent>()->RestartLevel();
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
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
		GameComponent(GameObject* pOwner)
			: Component(pOwner)
		{
			EventQueue::Instance().AddObserver(this);
			ServiceLocator::GetSoundSystem().Play(static_cast<sound_id>(Sounds::game_music), 1.f);
		}
		virtual ~GameComponent()
		{
			EventQueue::Instance().RemoveObserver(this);
		}

		virtual void Update() override
		{
		}
		virtual void Notify(std::unique_ptr<Event>& pEvent)
		{
			switch (pEvent->m_ID)
			{
			case "EventPlayerDied"_h:
			{
				SceneManager::Instance().CurrentScene()->GetObjectWithName("Grid"_h)->GetComponent<GridComponent>()->RestartLevel();
				break;
			}
			default:
				break;
			}
		}
	};
}

#endif
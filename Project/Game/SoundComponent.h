#ifndef SOUND_COMPONENT_H
#define SOUND_COMPONENT_H

#include "Component.h"
#include "Observer.h"
#include "Events.h"
#include "GameSounds.h"

namespace dae
{
	class SoundComponent final : public Component, public Observer
	{
	public:
		SoundComponent(GameObject* pOwner)
			: Component(pOwner)
		{
			EventQueue::Instance().AddObserver(this);
		}
		virtual ~SoundComponent()
		{
			EventQueue::Instance().RemoveObserver(this);
		}

		virtual void Update() override {}

		virtual void Notify(std::unique_ptr<Event>& pEvent) override
		{
			switch (pEvent->m_ID)
			{
			case "ScoreEvent"_h:
			{
				break;
			}
			case "EventGameOver"_h:
			{
				ServiceLocator::GetSoundSystem().StopAll();
				ServiceLocator::GetSoundSystem().Play(static_cast<sound_id>(Sounds::game_over), 1.f);
				break;
			}
			case "EventPlayerDied"_h:
			{
				break;
			}
			default:
				break;
			}
		}

	private:

	};
}

#endif

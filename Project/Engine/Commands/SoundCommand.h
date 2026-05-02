#ifndef SOUND_COMMAND_H
#define SOUND_COMMAND_H

#include "BaseCommand.h"
#include "SoundSystem.h"
#include "EventQueue.h"

namespace dae
{
	class SoundCommand final : public BaseCommand
	{
	public:
		SoundCommand(sound_id id, float volume)
			: m_ID{ id }
			, m_Volume{ volume }
		{
		}
		~SoundCommand() = default;
		virtual void Execute() override
		{
			std::unique_ptr<dae::Event> pSoundEvent{ std::make_unique<dae::EventSoundRequested>(m_ID, m_Volume) };
			dae::EventQueue::GetInstance().Enqueue(std::move(pSoundEvent));
		}

	private:
		sound_id m_ID;
		float m_Volume;
	};
}

#endif

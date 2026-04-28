#ifndef  SOUND_SYSTEM_H
#define  SOUND_SYSTEM_H

#include <cstdint>
#include <memory>
#include "Observer.h"

namespace dae
{
	using sound_id = uint16_t;

	class BaseSoundSystem : public Observer
	{
	public:
		virtual ~BaseSoundSystem() = default;
		virtual void Play(const sound_id id, const float volume) = 0;
		virtual void Notify(std::unique_ptr<Event>& pEvent) = 0;
	};

	class NullSoundSystem final : public BaseSoundSystem
	{
	public:
		virtual ~NullSoundSystem() = default;
		virtual void Play(const sound_id, const float)
		{
		}
		virtual void Notify(std::unique_ptr<Event>&) override
		{

		}
	};

	//sound events
	class EventSoundRequested final : public Event
	{
	public:
		EventSoundRequested(sound_id id, float volume)
			: Event(hash::make_sdbm_hash("SoundRequested"))
			, m_ID{ id }
			, m_Volume{ volume }
		{
		}
		const sound_id m_ID;
		const float m_Volume;
	};

	class SoundSystem : public BaseSoundSystem
	{
	public:
		SoundSystem(float masterVolume);
		virtual ~SoundSystem();
		virtual void Play(const sound_id id, const float volume = 1.f);
		virtual void Notify(std::unique_ptr<Event>& pEvent) override;

	protected:
		class SoundSystemImpl;
		std::unique_ptr<SoundSystemImpl> m_pImpl;
	};

	class LoggingSoundSystem final : public SoundSystem
	{
	public:
		virtual ~LoggingSoundSystem() = default;
	};
}

#endif

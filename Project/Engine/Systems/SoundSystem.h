#ifndef  SOUND_SYSTEM_H
#define  SOUND_SYSTEM_H

#include <iostream>
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
		virtual void Play(const sound_id, const float) override
		{
		}
		virtual void Notify(std::unique_ptr<Event>&) override
		{
		}
	};

	class SoundSystem : public BaseSoundSystem
	{
	public:
		SoundSystem(float masterVolume);
		virtual ~SoundSystem();
		virtual void Play(sound_id id, float volume = 1.f) override;
		virtual void Notify(std::unique_ptr<Event>& pEvent) override;

	protected:
		class SoundSystemImpl;
		std::unique_ptr<SoundSystemImpl> m_pImpl;
	};

	class LoggingSoundSystem final : public BaseSoundSystem
	{
	public:
		LoggingSoundSystem(std::unique_ptr<BaseSoundSystem>&& pSoundSystem) : m_pSoundSystem{ std::move(pSoundSystem) } {}
		virtual ~LoggingSoundSystem() = default;
		virtual void Play(sound_id id, float volume = 1.f) override
		{
			std::cout << "Playing sound with id " << id << " at volume " << volume << "\n";
			m_pSoundSystem->Play(id, volume);
		}
		virtual void Notify(std::unique_ptr<Event>& pEvent) override
		{
			m_pSoundSystem->Notify(pEvent);
		}

	private:
		std::unique_ptr<BaseSoundSystem> m_pSoundSystem;
	};

	//sound events
	class EventSoundRequested final : public Event
	{
	public:
		EventSoundRequested(sound_id id, float volume)
			: Event("SoundRequested"_h)
			, m_ID{ id }
			, m_Volume{ volume }
		{}
		const sound_id m_ID;
		const float m_Volume;
	};
}

#endif

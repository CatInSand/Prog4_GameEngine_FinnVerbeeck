#ifndef  SOUND_SYSTEM_H
#define  SOUND_SYSTEM_H

#include <cstdint>
#include <memory>
#include <unordered_map>

#include "Observer.h"
#include "BaseCommand.h"

namespace dae
{
	using sound_id = uint16_t;

	class BaseSoundSystem : public Observer
	{
	public:
		virtual ~BaseSoundSystem() = default;
		virtual void Play(const sound_id id, float volume) = 0;
		virtual void Stop(const sound_id id) = 0;
		virtual void StopAll() = 0;
		virtual void Notify(std::unique_ptr<Event>& pEvent) = 0;
		virtual void ToggleMute() = 0;
		virtual void LoadSoundMap(const std::unordered_map<sound_id, std::string>& map) = 0;
	};

	class NullSoundSystem final : public BaseSoundSystem
	{
	public:
		virtual ~NullSoundSystem() = default;
		virtual void Play(const sound_id, float) override {}
		virtual void Stop(const sound_id) override {}
		virtual void StopAll() override {}
		virtual void Notify(std::unique_ptr<Event>&) override {}
		virtual void ToggleMute() override {}
		virtual void LoadSoundMap(const std::unordered_map<sound_id, std::string>&) override {};
	};

	class SoundSystem : public BaseSoundSystem
	{
	public:
		SoundSystem(float masterVolume);
		virtual ~SoundSystem();
		virtual void Play(sound_id id, float volume = 1.f) override;
		virtual void Stop(const sound_id id) override;
		virtual void StopAll() override;
		virtual void Notify(std::unique_ptr<Event>& pEvent) override;
		virtual void ToggleMute() override;
		virtual void LoadSoundMap(const std::unordered_map<sound_id, std::string>& map) override;

	protected:
		class SoundSystemImpl;
		std::unique_ptr<SoundSystemImpl> m_pImpl;
	};

	class LoggingSoundSystem final : public BaseSoundSystem
	{
	public:
		LoggingSoundSystem(std::unique_ptr<BaseSoundSystem>&& pSoundSystem);
		virtual ~LoggingSoundSystem() = default;
		virtual void Play(sound_id id, float volume = 1.f) override;
		virtual void Stop(const sound_id id) override;
		virtual void StopAll() override;
		virtual void Notify(std::unique_ptr<Event>& pEvent) override;
		virtual void ToggleMute() override;
		virtual void LoadSoundMap(const std::unordered_map<sound_id, std::string>& map) override;

	private:
		std::unique_ptr<BaseSoundSystem> m_pSoundSystem;
	};

	class SoundCommand final : public BaseCommand
	{
	public:
		SoundCommand(sound_id id, float volume);
		virtual ~SoundCommand() = default;
		virtual void Execute() override;

	private:
		sound_id m_ID;
		float m_Volume;
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

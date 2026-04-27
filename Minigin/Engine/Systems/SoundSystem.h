#ifndef  SOUND_SYSTEM_H
#define  SOUND_SYSTEM_H

#include <cstdint>
#include <memory>

namespace dae
{
	using sound_id = uint16_t;

	class BaseSoundSystem
	{
	public:
		virtual ~BaseSoundSystem() = default;
		virtual void Play(const sound_id id, const float volume) = 0;
	};

	class NullSoundSystem final : public BaseSoundSystem
	{
	public:
		virtual void Play(const sound_id, const float)
		{
		}
	};

	class SoundSystem : public BaseSoundSystem
	{
	public:
		SoundSystem();
		virtual ~SoundSystem();
		virtual void Play(const sound_id id, const float volume = 1.f);

	protected:
		class SoundSystemImpl;
		std::unique_ptr<SoundSystemImpl> m_pImpl;
	};

	class LoggingSoundSystem final : public SoundSystem
	{
	public:
		virtual void Play(const sound_id id, const float volume = 1.f);
	};
}

#endif

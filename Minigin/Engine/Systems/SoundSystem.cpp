#include "SoundSystem.h"
#include "SDL3_mixer/SDL_mixer.h"

//https://wiki.libsdl.org/SDL3_mixer/CategorySDLMixer

namespace dae
{
	class SoundSystem::SoundSystemImpl final
	{
	public:
		SoundSystemImpl()
		{
			MIX_Init();
			m_pMixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
			m_pAudio = MIX_LoadAudio(m_pMixer, "./Data/spring.wav", false);
			m_pTrack = MIX_CreateTrack(m_pMixer);
			MIX_SetTrackAudio(m_pTrack, m_pAudio);
		}
		~SoundSystemImpl()
		{
			MIX_DestroyTrack(m_pTrack);
			MIX_DestroyAudio(m_pAudio);
			MIX_DestroyMixer(m_pMixer);
			MIX_Quit();
		}

		void Play(const sound_id, const float)
		{
			MIX_PlayTrack(m_pTrack, 0);
		}

	private:
		MIX_Mixer* m_pMixer{ nullptr };
		MIX_Audio* m_pAudio{ nullptr };
		MIX_Track* m_pTrack{ nullptr };
	};
}

dae::SoundSystem::SoundSystem()
	: m_pImpl{ std::make_unique<SoundSystemImpl>() }
{

}
dae::SoundSystem::~SoundSystem() = default;

void dae::SoundSystem::Play(const sound_id id , const float volume)
{
	//play sound using SDL_mixer
	m_pImpl->Play(id, volume);
}

void dae::LoggingSoundSystem::Play(const sound_id id, const float volume)
{
	//play sound using SDL_mixer with logging
	dae::SoundSystem::Play(id, volume);
}
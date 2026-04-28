#include "SoundSystem.h"
#include "SDL3_mixer/SDL_mixer.h"
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <unordered_map>

//https://wiki.libsdl.org/SDL3_mixer/CategorySDLMixer

constexpr float MIN_VOLUME{ 0.f };
constexpr float MAX_VOLUME{ 1.f };

namespace dae
{
	class SoundSystem::SoundSystemImpl final
	{
	public:
		SoundSystemImpl(float masterVolume)
			: m_MasterVolume{ std::clamp(masterVolume, MIN_VOLUME, MAX_VOLUME) }
		{
			MIX_Init();
			m_pMixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
			MIX_SetMixerGain(m_pMixer, m_MasterVolume);

			m_TrackPointers.reserve(TRACK_COUNT);
			for (int index{ 0 }; index < TRACK_COUNT; ++index)
			{
				m_TrackPointers.push_back(MIX_CreateTrack(m_pMixer));
			}
		}
		~SoundSystemImpl()
		{
			for (auto [id, pAudio] : m_IDAudioMap)
			{
				MIX_DestroyAudio(pAudio);
			}
			for (MIX_Track* pTrack : m_TrackPointers)
			{
				MIX_DestroyTrack(pTrack);
			}

			MIX_DestroyMixer(m_pMixer);
			MIX_Quit();
		}

		void Play(const sound_id id, float volume)
		{
			if (!m_IDPathMap.contains(id))
			{
				throw std::runtime_error("No sound with id:" + id);
			}

			if (MIX_Track* pTrack{ GetFreeTrack() }; pTrack != nullptr)
			{
				volume = std::clamp(volume, MIN_VOLUME, MAX_VOLUME);
				MIX_SetTrackGain(pTrack, volume);

				if (!m_IDAudioMap.contains(id))
				{
					m_IDAudioMap[id] = MIX_LoadAudio(m_pMixer, m_IDPathMap.at(id).c_str(), false);
				}
				MIX_SetTrackAudio(pTrack, m_IDAudioMap.at(id));

				MIX_PlayTrack(pTrack, 0);
			}
			else
			{
				throw std::runtime_error("Attempted to play sound while all tracks were playing");
			}
		}
		void Notify(std::unique_ptr<dae::Event>& pEvent)
		{
			if (pEvent->m_ID == hash::make_sdbm_hash("SoundRequested"))
			{
				EventSoundRequested* currentEvent{ reinterpret_cast<EventSoundRequested*>(pEvent.get()) };
				Play(currentEvent->m_ID, currentEvent->m_Volume);
			}
		}

	private:
		MIX_Track* GetFreeTrack()
		{
			for (MIX_Track* pTrack : m_TrackPointers)
			{
				if (!MIX_TrackPlaying(pTrack))
				{
					return pTrack;
				}
			}

			return nullptr;
		}

		MIX_Mixer* m_pMixer{ nullptr };
		float m_MasterVolume;

		static const uint8_t TRACK_COUNT{ 4 };
		std::vector<MIX_Track*> m_TrackPointers{};
		std::unordered_map<sound_id, MIX_Audio*> m_IDAudioMap{};
		static const std::unordered_map<sound_id, std::string> m_IDPathMap;
	};

	const std::unordered_map<sound_id, std::string> SoundSystem::SoundSystemImpl::m_IDPathMap{
		{0, "./Data/spring.wav"}
	};
}

dae::SoundSystem::SoundSystem(float masterVolume)
	: m_pImpl{ std::make_unique<SoundSystemImpl>(masterVolume) }
{

}
dae::SoundSystem::~SoundSystem() = default;
void dae::SoundSystem::Play(const sound_id id , const float volume)
{
	m_pImpl->Play(id, volume);
}
void dae::SoundSystem::Notify(std::unique_ptr<dae::Event>& pEvent)
{
	m_pImpl->Notify(pEvent);
}
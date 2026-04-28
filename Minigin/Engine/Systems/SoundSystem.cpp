#include "SoundSystem.h"
#include "SDL3_mixer/SDL_mixer.h"
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <unordered_map>
#include <mutex>

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

			m_Tracks.reserve(TRACK_COUNT);
			for (int index{ 0 }; index < TRACK_COUNT; ++index)
			{
				m_Tracks.emplace_back(MIX_CreateTrack(m_pMixer));
			}
		}
		~SoundSystemImpl()
		{
			for (auto [id, pAudio] : m_IDAudioMap)
			{
				MIX_DestroyAudio(pAudio);
			}
			for (auto& [pTrack, mutex] : m_Tracks)
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

			auto& [pTrack, mutex] { GetFreeTrack() };

			volume = std::clamp(volume, MIN_VOLUME, MAX_VOLUME);
			MIX_SetTrackGain(pTrack, volume);

			{
				std::lock_guard<std::mutex> lock(m_MixerMutex);
				if (!m_IDAudioMap.contains(id))
				{
					m_IDAudioMap[id] = MIX_LoadAudio(m_pMixer, m_IDPathMap.at(id).c_str(), false);
				}
				MIX_SetTrackAudio(pTrack, m_IDAudioMap.at(id));
			}

			MIX_PlayTrack(pTrack, 0);
			mutex.unlock();
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
		std::pair<MIX_Track*, std::mutex>& GetFreeTrack()
		{
			for (auto& track : m_Tracks)
			{
				if (track.second.try_lock())
				{
					if (!MIX_TrackPlaying(track.first))
					{
						return track;
					}
					track.second.unlock();
				}
			}

			throw std::runtime_error("Attempted to get free track while none were available");
		}

		MIX_Mixer* m_pMixer{ nullptr };
		std::mutex m_MixerMutex;
		float m_MasterVolume;

		static const uint8_t TRACK_COUNT{ 4 };
		//mutex locked from acquiring the track until it starts playing
		std::vector<std::pair<MIX_Track*, std::mutex>> m_Tracks{};
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
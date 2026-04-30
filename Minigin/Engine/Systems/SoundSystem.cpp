#include "SoundSystem.h"
#include "SDL3_mixer/SDL_mixer.h" 
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <unordered_map>
#include <mutex>
#include <queue>
#include <stop_token>
#include <thread>

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
				m_Tracks.push_back(MIX_CreateTrack(m_pMixer));
			}

			m_Thread = std::jthread{ &dae::SoundSystem::SoundSystemImpl::AudioMain, this, std::stop_token{} };
		}
		~SoundSystemImpl()
		{
			for (auto [id, pAudio] : m_IDAudioMap)
			{
				MIX_DestroyAudio(pAudio);
			}
			for (MIX_Track* pTrack : m_Tracks)
			{
				MIX_DestroyTrack(pTrack);
			}

			MIX_DestroyMixer(m_pMixer);
			MIX_Quit();
		}

		void Play(const sound_id id, float volume)
		{
			assert(m_IDPathMap.contains(id));

			std::unique_lock<std::mutex> lock{ m_Mutex };
			m_SoundQueue.emplace(id, volume);
			m_ConditionVariable.notify_all();
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
		void AudioMain(std::stop_token stopToken)
		{
			while (!stopToken.stop_requested())
			{
				std::unique_lock<std::mutex> lock{ m_Mutex };

				while (!stopToken.stop_requested() && m_SoundQueue.empty())
				{
					//wait until notified
					m_ConditionVariable.wait(lock);
				}
				while (!m_SoundQueue.empty())
				{
					auto [id, volume] { m_SoundQueue.front() };
					m_SoundQueue.pop();
					lock.unlock();

					MIX_Track* pTrack{ GetFreeTrack() };

					volume = std::clamp(volume, MIN_VOLUME, MAX_VOLUME);
					MIX_SetTrackGain(pTrack, volume);

					if (!m_IDAudioMap.contains(id))
					{
						m_IDAudioMap[id] = MIX_LoadAudio(m_pMixer, m_IDPathMap.at(id).c_str(), false);
					}
					MIX_SetTrackAudio(pTrack, m_IDAudioMap.at(id));

					MIX_PlayTrack(pTrack, 0);

					lock.lock();
				}
			}
		}
		MIX_Track* GetFreeTrack()
		{
			for (MIX_Track* pTrack : m_Tracks)
			{
				if (!MIX_TrackPlaying(pTrack))
				{
					return pTrack;
				}
			}

			throw std::runtime_error("Attempted to get free track while none were available");
		}

		std::condition_variable m_ConditionVariable{};
		std::jthread m_Thread{};
		std::mutex m_Mutex{};

		float m_MasterVolume;
		MIX_Mixer* m_pMixer{ nullptr };
		static const uint8_t TRACK_COUNT{ 4 };
		std::vector<MIX_Track*> m_Tracks{};

		std::queue<std::pair<sound_id, float>> m_SoundQueue{};

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
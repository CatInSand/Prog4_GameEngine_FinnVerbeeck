#include "SoundSystem.h"

#include <vector>
#include <stdexcept>
#include <algorithm>
#include <unordered_map>
#include <mutex>
#include <queue>
#include <stop_token>
#include <thread>
#include <iostream>

#include "SDL3_mixer/SDL_mixer.h"

#include "ResourceManager.h"
#include "ServiceLocator.h"

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

			m_Thread = std::jthread{ &dae::SoundSystem::SoundSystemImpl::AudioMain, this, m_StopToken };
		}
		~SoundSystemImpl()
		{
			//stop thread
			std::unique_lock<std::mutex> lock{ m_Mutex };
			m_StopSource.request_stop();
			m_ConditionVariable.notify_all();
			lock.unlock();

			//cleanup
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
			m_SoundQueue.emplace(id, OptionalMute(volume));
			m_ConditionVariable.notify_all();
		}
		void Notify(std::unique_ptr<dae::Event>& pEvent)
		{
			if (pEvent->m_ID == "SoundRequested"_h)
			{
				EventSoundRequested* currentEvent{ reinterpret_cast<EventSoundRequested*>(pEvent.get()) };
				Play(currentEvent->m_ID, currentEvent->m_Volume);
			}
		}
		void ToggleMute()
		{
			m_Muted = !m_Muted;
		}
		void LoadSoundMap(const std::unordered_map<sound_id, std::string>& map)
		{
			m_IDPathMap = map;
		}

	private:
		float OptionalMute(float volume)
		{
			return m_Muted ? 0.f : volume;
		}
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
						std::filesystem::path path{ ResourceManager::Instance().DataPath() / m_IDPathMap.at(id) };
						m_IDAudioMap[id] = MIX_LoadAudio(m_pMixer, path.string().c_str(), false);
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

			//no free tracks
			static int16_t currentStoppedTrack{ -1 };
			currentStoppedTrack = (currentStoppedTrack + 1) % TRACK_COUNT;
			MIX_StopTrack(m_Tracks[currentStoppedTrack], 0);
			return m_Tracks[currentStoppedTrack];
		}

		std::stop_source m_StopSource;
		std::stop_token m_StopToken{ m_StopSource.get_token() };
		std::condition_variable m_ConditionVariable{};
		std::jthread m_Thread{};
		std::mutex m_Mutex{};

		bool m_Muted{ false };
		float m_MasterVolume;
		MIX_Mixer* m_pMixer{ nullptr };
		static const uint8_t TRACK_COUNT{ 16 };
		std::vector<MIX_Track*> m_Tracks{};

		std::queue<std::pair<sound_id, float>> m_SoundQueue{};

		std::unordered_map<sound_id, MIX_Audio*> m_IDAudioMap{};
		std::unordered_map<sound_id, std::string> m_IDPathMap{};
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
void dae::SoundSystem::ToggleMute()
{
	m_pImpl->ToggleMute();
}
void dae::SoundSystem::LoadSoundMap(const std::unordered_map<sound_id, std::string>& map)
{
	m_pImpl->LoadSoundMap(map);
}

dae::LoggingSoundSystem::LoggingSoundSystem(std::unique_ptr<BaseSoundSystem>&& pSoundSystem)
	: m_pSoundSystem{ std::move(pSoundSystem) }
{
}
void dae::LoggingSoundSystem::Play(sound_id id, float volume)
{
	std::cout << "Playing sound with id " << id << " at volume " << volume << "\n";
	m_pSoundSystem->Play(id, volume);
}
void dae::LoggingSoundSystem::Notify(std::unique_ptr<Event>& pEvent)
{
	if (pEvent->m_ID == "SoundRequested"_h)
	{
		EventSoundRequested* currentEvent{ reinterpret_cast<EventSoundRequested*>(pEvent.get()) };
		std::cout << "Playing sound with id " << currentEvent->m_ID << " at volume " << currentEvent->m_Volume << "\n";
	}
	m_pSoundSystem->Notify(pEvent);
}
void dae::LoggingSoundSystem::ToggleMute()
{
	std::cout << "Toggling mute\n";
	m_pSoundSystem->ToggleMute();
}
void dae::LoggingSoundSystem::LoadSoundMap(const std::unordered_map<sound_id, std::string>& map)
{
	std::cout << "Loading sounds\n";
	m_pSoundSystem->LoadSoundMap(map);
}

dae::SoundCommand::SoundCommand(sound_id id, float volume)
	: m_ID{ id }
	, m_Volume{ volume }
{
}
void dae::SoundCommand::Execute()
{
	ServiceLocator::GetSoundSystem().Play(m_ID, m_Volume);
}
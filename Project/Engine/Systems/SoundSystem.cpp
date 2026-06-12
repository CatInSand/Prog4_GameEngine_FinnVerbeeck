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

constexpr float STOP_TRACK{ -1.f };
constexpr float MUTE_TRACK{ -2.f };
constexpr float UNMUTE_TRACK{ -3.f };
constexpr dae::sound_id ALL_TRACKS{ UINT16_MAX };

namespace dae
{
	class Track final
	{
	public:
		Track(MIX_Mixer* pMixer)
			: m_Track{ MIX_CreateTrack(pMixer) }
		{
		}
		~Track()
		{
			if (m_Track != nullptr)
			{
				MIX_DestroyTrack(m_Track);
			}
		}
		Track(const Track&) = delete;
		Track& operator=(const Track&) = delete;
		Track(Track&& other)
			: m_Track{ std::move(other.m_Track) }
			, m_ID{ std::move(other.m_ID) }
			, m_Volume{ std::move(other.m_Volume) }
			, m_Muted{ std::move(other.m_Muted) }
		{
			other.m_Track = nullptr;
		}
		Track& operator=(Track&& other)
		{
			m_Track = std::move(other.m_Track);
			m_ID = std::move(other.m_ID);
			m_Volume = std::move(other.m_Volume);
			m_Muted = std::move(other.m_Muted);
			return *this;
		}

		void SetAudio(MIX_Audio* pAudio, sound_id id, float volume = 1.f)
		{
			m_ID = id;
			m_Volume = volume;
			MIX_SetTrackAudio(m_Track, pAudio);
			if (!m_Muted)
			{
				MIX_SetTrackGain(m_Track, m_Volume);
			}
		}
		void Play()
		{
			MIX_PlayTrack(m_Track, 0);
		}
		void Stop()
		{
			MIX_StopTrack(m_Track, 0);
		}
		void Mute(bool mute)
		{
			if (m_Muted != mute)
			{
				m_Muted = !m_Muted;
				if (m_Muted)
				{
					MIX_SetTrackGain(m_Track, 0.f);
				}
				else
				{
					MIX_SetTrackGain(m_Track, m_Volume);
				}
			}
		}

		bool Playing() const
		{
			return MIX_TrackPlaying(m_Track);
		}
		sound_id ID() const
		{
			return m_ID;
		}

	private:
		MIX_Track* m_Track;
		sound_id m_ID{};
		float m_Volume{ 0.f };
		bool m_Muted{ false };
	};

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
				m_Tracks.push_back(Track{ m_pMixer });
			}

			m_Thread = std::jthread{ &dae::SoundSystem::SoundSystemImpl::AudioMain, this, m_StopToken };
		}
		~SoundSystemImpl()
		{
			//stop thread
			std::unique_lock<std::mutex> lock{ m_QueueMutex };
			m_StopSource.request_stop();
			m_ConditionVariable.notify_all();
			lock.unlock();

			//cleanup
			for (auto [id, pAudio] : m_IDAudioMap)
			{
				MIX_DestroyAudio(pAudio);
			}

			MIX_DestroyMixer(m_pMixer);
			MIX_Quit();
		}

		void Play(sound_id id, float volume)
		{
			assert(m_IDPathMap.contains(id));
			assert(volume >= 0.f);

			std::unique_lock<std::mutex> lock{ m_QueueMutex };
			m_SoundQueue.emplace(id, volume);
			m_ConditionVariable.notify_all();
		}
		void Stop(sound_id id)
		{
			assert(m_IDPathMap.contains(id));

			std::unique_lock<std::mutex> lock{ m_QueueMutex };
			m_SoundQueue.emplace(id, STOP_TRACK);
			m_ConditionVariable.notify_all();
		}
		void StopAll()
		{
			std::unique_lock<std::mutex> lock{ m_QueueMutex };
			m_SoundQueue.emplace(ALL_TRACKS, STOP_TRACK);
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
			std::unique_lock<std::mutex> lock{ m_QueueMutex };
			m_Muted = !m_Muted;
			if (m_Muted)
			{
				m_SoundQueue.emplace(ALL_TRACKS, MUTE_TRACK);
			}
			else
			{
				m_SoundQueue.emplace(ALL_TRACKS, UNMUTE_TRACK);
			}
			m_ConditionVariable.notify_all();
		}
		void LoadSoundMap(const std::unordered_map<sound_id, std::string>& map)
		{
			m_IDPathMap = map;
		}

	private:
		void AudioMain(std::stop_token stopToken)
		{
			while (!stopToken.stop_requested())
			{
				std::unique_lock<std::mutex> lock{ m_QueueMutex };

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

					if (volume >= 0.f)
					{
						Track* pTrack{ GetFreeTrack() };

						volume = std::clamp(volume, MIN_VOLUME, MAX_VOLUME);
						if (!m_IDAudioMap.contains(id))
						{
							std::filesystem::path path{ ResourceManager::Instance().DataPath() / m_IDPathMap.at(id) };
							m_IDAudioMap[id] = MIX_LoadAudio(m_pMixer, path.string().c_str(), false);
						}

						pTrack->SetAudio(m_IDAudioMap.at(id), id, volume);
						pTrack->Play();
					}
					else
					{
						for (Track& track : m_Tracks)
						{
							if (id == ALL_TRACKS || id == track.ID())
							{
								if (volume == STOP_TRACK)
									track.Stop();
								else if (volume == MUTE_TRACK)
									track.Mute(true);
								else if (volume == UNMUTE_TRACK)
									track.Mute(false);
							}
						}
					}

					lock.lock();
				}
			}
		}
		Track* GetFreeTrack()
		{
			for (Track& track : m_Tracks)
			{
				if (!track.Playing())
				{
					return &track;
				}
			}

			//no free tracks
			static int16_t currentStoppedTrack{ -1 };
			currentStoppedTrack = (currentStoppedTrack + 1) % TRACK_COUNT;
			m_Tracks[currentStoppedTrack].Stop();
			return &m_Tracks[currentStoppedTrack];
		}

		std::stop_source m_StopSource;
		std::stop_token m_StopToken{ m_StopSource.get_token() };
		std::condition_variable m_ConditionVariable{};
		std::jthread m_Thread{};
		std::mutex m_QueueMutex{};

		bool m_Muted{ false };
		float m_MasterVolume;
		MIX_Mixer* m_pMixer{ nullptr };
		static const uint8_t TRACK_COUNT{ 16 };
		std::vector<Track> m_Tracks{};

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
void dae::SoundSystem::Stop(const sound_id id)
{
	m_pImpl->Stop(id);
}
void dae::SoundSystem::StopAll()
{
	m_pImpl->StopAll();
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
void dae::LoggingSoundSystem::Stop(const sound_id id)
{
	std::cout << "Stopping sound with id " << id << "\n";
	m_pSoundSystem->Stop(id);
}
void dae::LoggingSoundSystem::StopAll()
{
	std::cout << "Stopping all sounds\n";
	m_pSoundSystem->StopAll();
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
#include "SoundSystem.h"

class SoundSystemImpl final
{
public:

private:

};

dae::SoundSystem::SoundSystem()
	: m_pImpl{ std::make_unique<SoundSystemImpl>() }
{

}
dae::SoundSystem::~SoundSystem() = default;

void dae::SoundSystem::Play(const sound_id id , const float volume)
{
	//play sound using SDL_mixer
}

void dae::LoggingSoundSystem::Play(const sound_id id, const float volume)
{
	//play sound using SDL_mixer with logging
	dae::SoundSystem::Play(id, volume);
}
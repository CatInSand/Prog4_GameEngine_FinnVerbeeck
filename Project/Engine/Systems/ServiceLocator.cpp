#include "ServiceLocator.h"

#include <cassert>

std::unique_ptr<dae::BaseSoundSystem> dae::ServiceLocator::m_pSoundSystemInstance{ std::make_unique<dae::NullSoundSystem>() };

dae::BaseSoundSystem& dae::ServiceLocator::GetSoundSystem()
{
	return *m_pSoundSystemInstance;
}
void dae::ServiceLocator::RegisterSoundSystem(std::unique_ptr<dae::BaseSoundSystem>&& pSoundSystem)
{
	m_pSoundSystemInstance = (pSoundSystem.get() == nullptr ? std::make_unique<NullSoundSystem>() : std::move(pSoundSystem));
}
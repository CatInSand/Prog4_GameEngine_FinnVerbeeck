#ifndef SERVICE_LOCATOR_H
#define SERVICE_LOCATOR_H

#include <memory>
#include "SoundSystem.h"

namespace dae
{
	class ServiceLocator final
	{
	public:
		static BaseSoundSystem& GetSoundSystem();
		static void RegisterSoundSystem(std::unique_ptr<BaseSoundSystem>&& pSoundSystem);

	private:
		static std::unique_ptr<BaseSoundSystem> m_pSoundSystemInstance;
	};
}

#endif

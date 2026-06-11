#ifndef GAME_COMMANDS_H
#define GAME_COMMANDS_H

#include "BaseCommand.h"
#include "Grid.h"
#include "ServiceLocator.h"
#include "Events.h"

namespace dae
{
	class NextLevelCommand final : public BaseCommand
	{
	public:
		NextLevelCommand(GridComponent* pGridComponent)
			: m_pGridComponent{ pGridComponent }
		{
		}
		virtual ~NextLevelCommand() = default;
		virtual void Execute()
		{
			m_pGridComponent->LoadNextLevel();
		}

	private:
		GridComponent* m_pGridComponent;
	};

	class ToggleMuteCommand final : public BaseCommand
	{
	public:
		ToggleMuteCommand()
		{}
		virtual ~ToggleMuteCommand() = default;
		virtual void Execute()
		{
			ServiceLocator::GetSoundSystem().ToggleMute();
		}
	};
}

#endif
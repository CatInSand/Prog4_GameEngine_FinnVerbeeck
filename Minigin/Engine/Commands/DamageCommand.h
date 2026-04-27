#ifndef DAMAGE_COMMAND_H
#define DAMAGE_COMMAND_H

#include "BaseCommand.h"
#include "HealthComponent.h"

namespace dae
{
	class DamageCommand final : public BaseCommand
	{
	public:
		DamageCommand(HealthComponent* pTarget);
		virtual void Execute() override;
	private:
		HealthComponent* m_pTarget;
	};
}

#endif
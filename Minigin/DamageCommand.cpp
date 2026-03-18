#include "DamageCommand.h"

dae::DamageCommand::DamageCommand(dae::HealthComponent* pTarget)
	: m_pTarget{ pTarget }
{

}
void dae::DamageCommand::Execute()
{
	m_pTarget->Damage(1);
}
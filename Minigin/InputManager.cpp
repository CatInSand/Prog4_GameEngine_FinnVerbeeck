#include "InputManager.h"

bool dae::InputManager::ProcessInput()
{
	if (!m_InputState.UpdateStates())
	{
		return false;
	}

	Execute();

	return true;
}

void dae::InputManager::AddKeyBind(const dae::KeyTrigger& keyTrigger, std::unique_ptr<dae::BaseCommand>&& pCommand)
{
	m_KeyBinds.push_back(dae::KeyBind(keyTrigger, std::move(pCommand)));
}
void dae::InputManager::RemoveKeyBind(const dae::KeyTrigger& keyTrigger)
{
	m_KeyBinds.erase(
		std::remove_if(
			m_KeyBinds.begin(), m_KeyBinds.end(),
			[&](dae::KeyBind& keyBind) { return keyBind.HasKeyTrigger(keyTrigger); }),
		m_KeyBinds.end()
	);
}

void dae::InputManager::Execute()
{
	for (dae::KeyBind& keyBind : m_KeyBinds)
	{
		dae::KeyTrigger keyTrigger{
			keyBind.Trigger().m_Scancode,
			m_InputState.GetKeyState(keyBind.Trigger().m_Scancode)
		};

		keyBind.ExecuteIfMatch(keyTrigger);
	}
}
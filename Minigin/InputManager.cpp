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
		dae::KeyTrigger keyTrigger{ keyBind.Trigger() };
		if (std::holds_alternative<SDL_Scancode>(keyTrigger.m_Code))
		{
			keyTrigger.m_Keystate = m_InputState.GetKeyState(std::get<SDL_Scancode>(keyTrigger.m_Code));
		}
		else
		{
#if !__EMSCRIPTEN__
			keyTrigger.m_Keystate = m_InputState.GetButtonState(std::get<unsigned int>(keyTrigger.m_Code));
#else
			keyTrigger.m_Keystate = m_InputState.GetButtonState(static_cast<unsigned int>(std::get<SDL_GamepadButton>(keyTrigger.m_Code)));
#endif
		}

		keyBind.ExecuteIfMatch(keyTrigger);
	}
}
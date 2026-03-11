#include "InputState.h"
#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>
#include <windows.h>
#include <Xinput.h>

class dae::InputState::GamepadImplementation final
{
public:
	GamepadImplementation() = default;
	dae::KeyState GetButtonState(unsigned int button) const
	{
		if (m_ButtonsPressedThisFrame & button)
			return dae::KeyState::down;
		else if (m_ButtonsReleasedThisFrame & button)
			return dae::KeyState::up;
		else if (m_GamepadStateCurrent.Gamepad.wButtons & button)
			return dae::KeyState::pressed;
		else
			return dae::KeyState::none;
	}
	void UpdateStates()
	{
		CopyMemory(&m_GamepadStatePrevious, &m_GamepadStateCurrent, sizeof(XINPUT_STATE));
		ZeroMemory(&m_GamepadStateCurrent, sizeof(XINPUT_STATE));
		XInputGetState(m_ControllerIndex, &m_GamepadStateCurrent);

		auto buttonChanges = m_GamepadStateCurrent.Gamepad.wButtons ^ m_GamepadStatePrevious.Gamepad.wButtons;
		m_ButtonsPressedThisFrame = buttonChanges & m_GamepadStateCurrent.Gamepad.wButtons;
		m_ButtonsReleasedThisFrame = buttonChanges & (~m_GamepadStateCurrent.Gamepad.wButtons);
	}

private:
	XINPUT_STATE m_GamepadStateCurrent{};
	XINPUT_STATE m_GamepadStatePrevious{};
	unsigned int m_ButtonsPressedThisFrame{};
	unsigned int m_ButtonsReleasedThisFrame{};
	uint8_t m_ControllerIndex{ 0 };
};

dae::InputState::InputState()
	: m_pImpl{ std::make_unique<GamepadImplementation>() }
{

}
dae::InputState::~InputState() = default;

bool dae::InputState::UpdateStates()
{
	//update keyboardstate
	for (auto& [scancode, keyState] : m_KeyBoardState)
	{
		switch (keyState)
		{
		case dae::KeyState::down:
			keyState = dae::KeyState::pressed;
			break;
		case dae::KeyState::up:
			keyState = dae::KeyState::none;
			break;
		default:
			break;
		}
	}

	//keyboard input
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_EVENT_QUIT:
			return false;
		case SDL_EVENT_KEY_UP:
			m_KeyBoardState[e.key.scancode] = dae::KeyState::up;
			break;
		case SDL_EVENT_KEY_DOWN:
			m_KeyBoardState[e.key.scancode] = dae::KeyState::down;
			break;
		default:
			break;
		}

		//process event for IMGUI
		ImGui_ImplSDL3_ProcessEvent(&e);
	}

	m_pImpl->UpdateStates();

	return true;
}

dae::KeyState dae::InputState::GetKeyState(SDL_Scancode scancode)
{
	return m_KeyBoardState[scancode];
}
dae::KeyState dae::InputState::GetButtonState(unsigned int button) const
{
	return m_pImpl->GetButtonState(button);
}
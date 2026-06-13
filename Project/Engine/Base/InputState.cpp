#include "InputState.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_gamepad.h>
#include <iostream>

dae::InputState::InputState()
{
	SDL_Init(SDL_INIT_GAMEPAD);
	if (!SDL_HasGamepad())
	{
		std::cout << "No gamepad detected\n";
	}
	int gamepadCount{};
	SDL_JoystickID* gamepads{ SDL_GetGamepads(&gamepadCount) };
	m_pGamepad = SDL_OpenGamepad(gamepads[0]);
}
dae::InputState::~InputState()
{
	SDL_CloseGamepad(m_pGamepad);
}

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

	for (auto& [button, keyState] : m_GamepadState)
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
		case SDL_EVENT_GAMEPAD_BUTTON_UP:
			m_GamepadState[static_cast<SDL_GamepadButton>(e.gbutton.button)] = dae::KeyState::up;
			break;
		case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
			m_GamepadState[static_cast<SDL_GamepadButton>(e.gbutton.button)] = dae::KeyState::down;
			break;
		default:
			break;
		}
	}

	return true;
}

dae::KeyState dae::InputState::GetKeyState(SDL_Scancode scancode)
{
	return m_KeyBoardState[scancode];
}

dae::KeyState dae::InputState::GetButtonState(SDL_GamepadButton button)
{
	return m_GamepadState[button];
}
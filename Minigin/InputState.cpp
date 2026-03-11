#include "InputState.h"
#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>

bool dae::InputState::UpdateStates()
{
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

	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_EVENT_QUIT:
			return false;
		case SDL_EVENT_KEY_UP:
			ReleaseKey(e.key.scancode);
			break;
		case SDL_EVENT_KEY_DOWN:
			PressKey(e.key.scancode);
			break;
		default:
			break;
		}

		//process event for IMGUI
		ImGui_ImplSDL3_ProcessEvent(&e);
	}

	return true;
}
void dae::InputState::PressKey(SDL_Scancode scancode)
{
	m_KeyBoardState[scancode] = dae::KeyState::down;
}
void dae::InputState::ReleaseKey(SDL_Scancode scancode)
{
	m_KeyBoardState[scancode] = dae::KeyState::up;
}

dae::KeyState dae::InputState::GetKeyState(SDL_Scancode scancode)
{
	return m_KeyBoardState[scancode];
}
#ifndef INPUT_STATE_H
#define INPUT_STATE_H

#include "KeyBind.h"
#include <unordered_map>
#include <memory>

namespace dae
{
	class InputState final
	{
	public:
		InputState();
		~InputState();
		
		bool UpdateStates();

		KeyState GetKeyState(SDL_Scancode scancode);
		KeyState GetButtonState(SDL_GamepadButton button);

	private:
		std::unordered_map<SDL_Scancode, KeyState> m_KeyBoardState{};
		std::unordered_map<SDL_GamepadButton, KeyState> m_GamepadState{};

		SDL_Gamepad* m_pGamepad{};
	};
}

#endif
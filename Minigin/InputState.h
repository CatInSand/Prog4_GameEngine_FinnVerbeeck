#ifndef INPUT_STATE_H
#define INPUT_STATE_H

#include "KeyBind.h"
#include <windows.h>
#include <Xinput.h>

#include <unordered_map>

namespace dae
{
	class InputState final
	{
	public:
		InputState() = default;
		
		bool UpdateStates();

		KeyState GetKeyState(SDL_Scancode scancode);
		KeyState GetButtonState(unsigned int button) const;

	private:
		std::unordered_map<SDL_Scancode, KeyState> m_KeyBoardState{};
		XINPUT_STATE m_GamepadStateCurrent{};
		XINPUT_STATE m_GamepadStatePrevious{};
		unsigned int m_ButtonsPressedThisFrame{};
		unsigned int m_ButtonsReleasedThisFrame{};

		uint8_t m_ControllerIndex{ 0 };
	};
}

#endif
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
		KeyState GetButtonState(unsigned int button) const;

	private:
		std::unordered_map<SDL_Scancode, KeyState> m_KeyBoardState{};

		class GamepadImplementation;
		std::unique_ptr<GamepadImplementation> m_pImpl;
	};
}

#endif
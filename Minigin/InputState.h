#ifndef INPUT_STATE_H
#define INPUT_STATE_H

#include "KeyBind.h"
//#include "Xinput.h"

#include <unordered_map>

namespace dae
{
	class InputState final
	{
	public:
		InputState() = default;
		
		bool UpdateStates();
		void PressKey(SDL_Scancode scancode);
		void ReleaseKey(SDL_Scancode scancode);

		KeyState GetKeyState(SDL_Scancode scancode);

	private:
		std::unordered_map<SDL_Scancode, KeyState> m_KeyBoardState{};

	};
}

#endif
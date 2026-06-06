#ifndef KEY_BIND_H
#define KEY_BIND_H

#include <memory>
#include "BaseCommand.h"
#include <SDL3/SDL.h>
#include <variant>

namespace dae
{
	enum class KeyState : char {
		none,
		down,
		up,
		pressed
	};
	struct KeyTrigger final
	{
		KeyTrigger(SDL_Scancode scancode, dae::KeyState keystate)
			: m_Keystate{ keystate }, m_Code{ scancode }
		{}
		KeyTrigger(SDL_GamepadButton button, dae::KeyState keystate)
			: m_Keystate{ keystate }, m_Code{ button }
		{
		}
		dae::KeyState m_Keystate;
		std::variant<SDL_Scancode, SDL_GamepadButton> m_Code;
	};

	class KeyBind
	{
	public:
		KeyBind(KeyTrigger keyTrigger, std::unique_ptr<BaseCommand>&& pCommand)
			: m_KeyTrigger{ keyTrigger }
			, m_pCommand{ std::move(pCommand) }
		{
		}
		bool HasKeyTrigger(const KeyTrigger& keyTrigger)
		{
			return keyTrigger.m_Keystate == m_KeyTrigger.m_Keystate &&
				keyTrigger.m_Code == m_KeyTrigger.m_Code;
		}
		void ExecuteIfMatch(KeyTrigger keyTrigger)
		{
			if (HasKeyTrigger(keyTrigger))
			{
				m_pCommand->Execute();
			}
		}
		KeyTrigger Trigger() const { return m_KeyTrigger; }

	private:
		KeyTrigger m_KeyTrigger;
		std::unique_ptr<BaseCommand> m_pCommand;
	};
}

#endif
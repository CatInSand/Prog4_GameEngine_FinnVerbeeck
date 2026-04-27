#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "Singleton.h"
#include <vector>
#include "KeyBind.h"
#include "InputState.h"

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		void AddKeyBind(const KeyTrigger& keyTrigger, std::unique_ptr<BaseCommand>&& pCommand);
		void RemoveKeyBind(const KeyTrigger& keyTrigger);

	private:
		void Execute();
		std::vector<KeyBind> m_KeyBinds{};
		InputState m_InputState{};
	};
}

#endif

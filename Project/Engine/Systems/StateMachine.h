#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <vector>
#include <memory>
#include <cassert>
#include <algorithm>
#include <concepts>
#include "Component.h"

namespace dae
{
	class GameObject;
	class State
	{
	public:
		virtual ~State() = default;
		virtual void Update() = 0;

		virtual void Enter() = 0;
		virtual void Exit() = 0;

	protected:
		State(GameObject* pObject);
		GameObject* m_pObject;
	};

	class StateMachine final : public Component
	{
	public:
		StateMachine(GameObject* owner, std::vector<std::unique_ptr<State>>&& states, uint8_t startState);
		~StateMachine() = default;
		StateMachine(const StateMachine& other) = delete;
		StateMachine(StateMachine&& other) = delete;
		StateMachine& operator=(const StateMachine& other) = delete;
		StateMachine& operator=(StateMachine&& other) = delete;

		virtual void Update() override;

		template<typename T>
			requires requires () { std::derived_from<T, State>; }
		void SetState()
		{
			auto it = std::find_if(m_States.begin(), m_States.end(),
				[](const std::unique_ptr<State>& pState) { return dynamic_cast<T*>(pState.get()) != nullptr; }
			);

			assert(it != m_pComponents.end());
			m_CurrentState->Exit();
			m_CurrentState = dynamic_cast<T*>((*it).get());
			m_CurrentState->Enter();
		}

	private:
		bool AllStatesUnique();
		std::vector<std::unique_ptr<State>> m_States;
		State* m_CurrentState{ nullptr };
	};
}

#endif
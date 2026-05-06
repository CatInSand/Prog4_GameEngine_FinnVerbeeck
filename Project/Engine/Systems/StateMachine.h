#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <vector>
#include <memory>
#include <cassert>
#include <algorithm>
#include <concepts>
#include "Component.h"
#include "GameObject.h"

namespace dae
{
	template<typename dataT>
	class State
	{
	public:
		virtual ~State() = default;
		virtual void Update() = 0;

		virtual void Enter() = 0;
		virtual void Exit() = 0;

	protected:
		State(dataT* pData)
			: m_pData{ pData }
		{
		}
		dataT* m_pData;
	};

	template<typename dataT>
	class StateMachine final : public Component
	{
	public:
		StateMachine(GameObject* owner, std::vector<std::unique_ptr<State<dataT>>>&& states, std::unique_ptr<dataT>&& pData, uint8_t startState)
			: Component(owner)
			, m_States{ std::move(states) }
			, m_pData{ std::move(pData) }
		{
			assert(startState < m_States.size());
			assert(AllStatesUnique());

			m_CurrentState = m_States[startState].get();
			m_CurrentState->Enter();
		}
		~StateMachine() = default;
		StateMachine(const StateMachine& other) = delete;
		StateMachine(StateMachine&& other) = delete;
		StateMachine& operator=(const StateMachine& other) = delete;
		StateMachine& operator=(StateMachine&& other) = delete;

		virtual void Update() override
		{
			m_CurrentState->Update();
		}

		template<typename T>
			requires std::derived_from<T, State<dataT>>
		void SetState()
		{
			auto it = std::find_if(m_States.begin(), m_States.end(),
				[](const std::unique_ptr<State<dataT>>& pState) { return dynamic_cast<T*>(pState.get()) != nullptr; }
			);

			assert(it != m_States.end());
			m_CurrentState->Exit();
			m_CurrentState = dynamic_cast<T*>((*it).get());
			m_CurrentState->Enter();
		}

		State<dataT>* CurrentState() const
		{
			return m_CurrentState;
		}

	private:
		bool AllStatesUnique()
		{
			int firstIndex{ 0 };
			for (std::unique_ptr<State<dataT>>& pState : m_States)
			{
				for (int secondIndex{ firstIndex + 1 }; secondIndex < m_States.size(); ++secondIndex)
				{
					//for each pair of states
					if (typeid(*pState.get()) == typeid(*m_States[secondIndex].get()))
					{
						//duplicate state
						return false;
					}
				}
				++firstIndex;
			}

			return true;
		}
		std::vector<std::unique_ptr<State<dataT>>> m_States;
		std::unique_ptr<dataT> m_pData;
		State<dataT>* m_CurrentState{ nullptr };
	};
}

#endif

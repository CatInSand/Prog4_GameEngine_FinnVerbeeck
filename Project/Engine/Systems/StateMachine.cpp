#include "StateMachine.h"
#include "GameObject.h"

dae::State::State(GameObject* pObject)
	: m_pObject{ pObject }
{
}

dae::StateMachine::StateMachine(GameObject* owner, std::vector<std::unique_ptr<State>>&& states, uint8_t startState)
	: Component(owner)
	, m_States{ std::move(states) }
{
	assert(startState < m_States.size());
	assert(AllStatesUnique());

	m_CurrentState = m_States[startState].get();
	m_CurrentState->Enter();
}

void dae::StateMachine::Update()
{
	m_CurrentState->Update();
}

dae::State* dae::StateMachine::CurrentState() const
{
	return m_CurrentState;
}

bool dae::StateMachine::AllStatesUnique()
{
	int firstIndex{ 0 };
	for (std::unique_ptr<State>& pState : m_States)
	{
		for (int secondIndex{ firstIndex + 1 }; secondIndex < m_States.size(); ++secondIndex)
		{
			//for each pair of states
			if (typeid(pState.get()) == typeid(m_States[secondIndex].get()))
			{
				//duplicate state
				return false;
			}
		}
		++firstIndex;
	}

	return true;
}
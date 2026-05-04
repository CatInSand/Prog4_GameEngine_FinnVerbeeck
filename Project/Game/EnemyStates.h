#ifndef ENEMY_STATES_H
#define ENEMY_STATE_H

#include "StateMachine.h"

namespace dae
{
	namespace Pooka
	{
		class IdleState final : public State
		{
		public:
			IdleState(GameObject* pObject)
				: State(pObject)
			{
			}
			virtual void Update() override {};

			virtual void Enter() override {};
			virtual void Exit() override {};
		};
	}
}

#endif

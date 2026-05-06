#ifndef POOKA_H
#define POOKA_H

#include "Component.h"
#include "StateMachine.h"
#include "Timer.h"

namespace dae
{
	namespace Pooka
	{
		enum class Sprites {
			walk,
			ghost,
			flat,
			blow,
		};

		struct StateData
		{
		public:
			StateData(GameObject* pPookaObject);

			GameObject* pPooka;
			GameObject* pPlayer;
			Timer timer{ 10.f };
		};

		using StateMachine = dae::StateMachine<StateData>;
		using State = dae::State<StateData>;

		class Idle final : public Pooka::State
		{
		public:
			Idle(StateData* pData) : Pooka::State(pData) {}
			virtual void Update() override;

			virtual void Enter() override;
			virtual void Exit() override;

		private:
			//float m_DetectionRadius{ 64.f };
		};
		class Ghost final : public Pooka::State
		{
		public:
			Ghost(StateData* pData) : Pooka::State(pData) {}
			virtual void Update() override;

			virtual void Enter() override;
			virtual void Exit() override;
		};
		class Chase final : public Pooka::State
		{
		public:
			Chase(StateData* pData) : Pooka::State(pData) {}
			virtual void Update() override;

			virtual void Enter() override;
			virtual void Exit() override;
		};
		class Flat final : public Pooka::State
		{
		public:
			Flat(StateData* pData) : Pooka::State(pData) {}
			virtual void Update() override;

			virtual void Enter() override;
			virtual void Exit() override;
		};
		class Blow final : public Pooka::State
		{
		public:
			Blow(StateData* pData) : Pooka::State(pData) {}
			virtual void Update() override;

			virtual void Enter() override;
			virtual void Exit() override;
		};
		class Deflate final : public Pooka::State
		{
		public:
			Deflate(StateData* pData) : Pooka::State(pData) {}
			virtual void Update() override;

			virtual void Enter() override;
			virtual void Exit() override;
		};
	}
}

#endif

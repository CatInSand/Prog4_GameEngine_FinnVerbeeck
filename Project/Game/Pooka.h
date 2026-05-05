#ifndef POOKA_H
#define POOKA_H

#include "Component.h"
#include "StateMachine.h"
#include "Timer.h"

namespace dae
{
	class Pooka final : public Component
	{
	public:
		enum class Sprites {
			walk,
			ghost,
			flat,
			blow,
		};

		Pooka(GameObject* pOwner);
		virtual ~Pooka() = default;
		Pooka(const Pooka& other) = delete;
		Pooka(Pooka&& other) = delete;
		Pooka& operator=(const Pooka& other) = delete;
		Pooka& operator=(Pooka&& other) = delete;

		virtual void Update() override;

		class Idle final : public State
		{
		public:
			Idle(GameObject* pObject);
			virtual void Update() override;

			virtual void Enter() override;
			virtual void Exit() override;
		private:
			Timer m_Timer{ 5.f };
		};
		class Ghost final : public State
		{
		public:
			Ghost(GameObject* pObject);
			virtual void Update() override;

			virtual void Enter() override;
			virtual void Exit() override;
		};
		class Chase final : public State
		{
		public:
			Chase(GameObject* pObject);
			virtual void Update() override;

			virtual void Enter() override;
			virtual void Exit() override;
		};
		class Flat final : public State
		{
		public:
			Flat(GameObject* pObject);
			virtual void Update() override;

			virtual void Enter() override;
			virtual void Exit() override;
		};
		class Blow final : public State
		{
		public:
			Blow(GameObject* pObject);
			virtual void Update() override;

			virtual void Enter() override;
			virtual void Exit() override;
		};

	private:
		StateMachine* m_pStateMachine{ nullptr };
	};
}

#endif

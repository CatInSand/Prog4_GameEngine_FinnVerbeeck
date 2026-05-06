#include "Pooka.h"
#include "GameObject.h"
#include "MultiSpriteComponent.h"

dae::Pooka::Pooka(dae::GameObject* pOwner)
	: Component(pOwner)
{
	m_pStateMachine = GetOwner()->GetComponent<StateMachine>();
}

void dae::Pooka::Update()
{

}


dae::Pooka::Idle::Idle(GameObject* pObject)
	: State(pObject)
{
}
void dae::Pooka::Idle::Update()
{
	m_Timer.Update();
	if (m_Timer.Done())
	{
		m_Timer.Reset();
		m_pObject->GetComponent<StateMachine>()->SetState<Ghost>();
	}
}
void dae::Pooka::Idle::Enter()
{
	m_pObject->GetComponent<MultiSpriteComponent>()->SetSprite(Sprites::walk);
}
void dae::Pooka::Idle::Exit()
{
}

dae::Pooka::Ghost::Ghost(GameObject* pObject)
	: State(pObject)
{
}
void dae::Pooka::Ghost::Update()
{
	m_Timer.Update();
	if (m_Timer.Done())
	{
		m_Timer.Reset();
		m_pObject->GetComponent<StateMachine>()->SetState<Chase>();
	}
}
void dae::Pooka::Ghost::Enter()
{
	m_pObject->GetComponent<MultiSpriteComponent>()->SetSprite(Sprites::ghost);
}
void dae::Pooka::Ghost::Exit()
{
}

dae::Pooka::Chase::Chase(GameObject* pObject)
	: State(pObject)
{
}
void dae::Pooka::Chase::Update()
{
	m_Timer.Update();
	if (m_Timer.Done())
	{
		m_Timer.Reset();
		m_pObject->GetComponent<StateMachine>()->SetState<Flat>();
	}
}
void dae::Pooka::Chase::Enter()
{
	m_pObject->GetComponent<MultiSpriteComponent>()->SetSprite(Sprites::walk);
}
void dae::Pooka::Chase::Exit()
{
}

dae::Pooka::Flat::Flat(GameObject* pObject)
	: State(pObject)
{
}
void dae::Pooka::Flat::Update()
{
}
void dae::Pooka::Flat::Enter()
{
	m_pObject->GetComponent<MultiSpriteComponent>()->SetSprite(Sprites::flat);
}
void dae::Pooka::Flat::Exit()
{
}

dae::Pooka::Blow::Blow(GameObject* pObject)
	: State(pObject)
{
}
void dae::Pooka::Blow::Update()
{
}
void dae::Pooka::Blow::Enter()
{
	m_pObject->GetComponent<MultiSpriteComponent>()->SetSprite(Sprites::blow);
}
void dae::Pooka::Blow::Exit()
{
}
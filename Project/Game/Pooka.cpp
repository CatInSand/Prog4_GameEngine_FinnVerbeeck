#include "Pooka.h"
#include "GameObject.h"
#include "MultiSpriteComponent.h"
#include "SceneManager.h"

dae::Pooka::StateData::StateData(GameObject* pPookaObject)
	: pPooka{ pPookaObject }
	, pPlayer{ SceneManager::GetInstance().CurrentScene()->GetObjectWithName("Player") }
{
}

void dae::Pooka::Idle::Update()
{
	// move idle
	// if player is nearby and reachable, enter chase state
	// if player is nearby and not reachable, enter ghost state
	m_pData->timer.Update();
	if (m_pData->timer.Done())
	{
		m_pData->timer.Reset();
		m_pData->pPooka->GetComponent<Pooka::StateMachine>()->SetState<Ghost>();
	}
}
void dae::Pooka::Idle::Enter()
{
	m_pData->pPooka->GetComponent<MultiSpriteComponent>()->SetSprite(Sprites::walk);
}
void dae::Pooka::Idle::Exit()
{
}

void dae::Pooka::Ghost::Update()
{
	// phase towards player until reach empty space, then enter chase
	m_pData->timer.Update();
	if (m_pData->timer.Done())
	{
		m_pData->timer.Reset();
		m_pData->pPooka->GetComponent<Pooka::StateMachine>()->SetState<Chase>();
	}
}
void dae::Pooka::Ghost::Enter()
{
	m_pData->pPooka->GetComponent<MultiSpriteComponent>()->SetSprite(Sprites::ghost);
}
void dae::Pooka::Ghost::Exit()
{
}

void dae::Pooka::Chase::Update()
{
	// follow player until hit or die
	m_pData->timer.Update();
	if (m_pData->timer.Done())
	{
		m_pData->timer.Reset();
		m_pData->pPooka->GetComponent<Pooka::StateMachine>()->SetState<Flat>();
	}
}
void dae::Pooka::Chase::Enter()
{
	m_pData->pPooka->GetComponent<MultiSpriteComponent>()->SetSprite(Sprites::walk);
}
void dae::Pooka::Chase::Exit()
{
}

void dae::Pooka::Flat::Update()
{
	// fall down until hit ground, then die
	m_pData->timer.Update();
	if (m_pData->timer.Done())
	{
		m_pData->timer.Reset();
		m_pData->pPooka->GetComponent<Pooka::StateMachine>()->SetState<Blow>();
	}
}
void dae::Pooka::Flat::Enter()
{
	m_pData->pPooka->GetComponent<MultiSpriteComponent>()->SetSprite(Sprites::flat);
}
void dae::Pooka::Flat::Exit()
{
}

void dae::Pooka::Blow::Update()
{
	// if no longer being hit, enter deflate state
	// else blow up further
	m_pData->timer.Update();
	if (m_pData->timer.Done())
	{
		m_pData->timer.Reset();
		m_pData->pPooka->GetComponent<Pooka::StateMachine>()->SetState<Deflate>();
	}
}
void dae::Pooka::Blow::Enter()
{
	m_pData->pPooka->GetComponent<MultiSpriteComponent>()->SetSprite(Sprites::blow);
}
void dae::Pooka::Blow::Exit()
{
}

void dae::Pooka::Deflate::Update()
{
	// deflate until normal, then enter idle
	m_pData->timer.Update();
	if (m_pData->timer.Done())
	{
		m_pData->timer.Reset();
		m_pData->pPooka->GetComponent<Pooka::StateMachine>()->SetState<Idle>();
	}
}
void dae::Pooka::Deflate::Enter()
{
	m_pData->pPooka->GetComponent<MultiSpriteComponent>()->SetSprite(Sprites::blow);
}
void dae::Pooka::Deflate::Exit()
{
}
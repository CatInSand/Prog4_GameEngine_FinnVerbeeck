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
}
void dae::Pooka::Blow::Enter()
{
	m_pData->pPooka->GetComponent<MultiSpriteComponent>()->SetSprite(Sprites::blow);
}
void dae::Pooka::Blow::Exit()
{
}
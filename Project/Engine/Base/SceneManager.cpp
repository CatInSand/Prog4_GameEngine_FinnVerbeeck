#include "SceneManager.h"
#include "Scene.h"
#include "EventQueue.h"

dae::Scene& dae::SceneManager::CreateScene()
{
	m_Scenes.push_back(std::make_unique<Scene>());
	if (m_CurrentScene == nullptr)
	{
		m_CurrentScene = m_Scenes[0].get();
	}
	return *m_Scenes.back();
}

dae::Scene* dae::SceneManager::CurrentScene()
{
	return m_CurrentScene;
}

void dae::SceneManager::Update()
{
	dae::EventQueue::GetInstance().SendAll();
	m_CurrentScene->Update();
}

void dae::SceneManager::Render()
{
	m_CurrentScene->Render();
}
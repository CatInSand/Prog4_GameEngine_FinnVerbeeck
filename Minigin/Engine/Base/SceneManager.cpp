#include "SceneManager.h"
#include "Scene.h"
#include "EventQueue.h"

void dae::SceneManager::Update()
{
	dae::EventQueue::GetInstance().SendAll();
	for(auto& scene : m_scenes)
	{
		scene->Update();
	}
}

void dae::SceneManager::Render()
{
	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}
}

dae::Scene& dae::SceneManager::CreateScene()
{
	m_scenes.emplace_back(new Scene());
	return *m_scenes.back();
}

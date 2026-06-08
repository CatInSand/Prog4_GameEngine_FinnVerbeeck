#include "SceneManager.h"

#include <format>

#include "Scene.h"
#include "EventQueue.h"
#include "CollisionQueue.h"

dae::Scene& dae::SceneManager::CreateScene()
{
	return CreateScene(std::format("Scene{}", m_Scenes.size() + 1));
}
dae::Scene& dae::SceneManager::CreateScene(const std::string& sceneName)
{
	m_Scenes.push_back(std::make_unique<Scene>(sceneName));
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

void dae::SceneManager::SetScene(const std::string& sceneName)
{
	for (std::unique_ptr<Scene>& pScene : m_Scenes)
	{
		if (pScene->Name() == sceneName)
		{
			m_CurrentScene = pScene.get();
			return;
		}
	}
	assert(false && "Could not change to scene with unknown name");
}
void dae::SceneManager::SetScene(size_t index)
{
	assert(index < m_Scenes.size());
	m_CurrentScene = m_Scenes[index].get();
}

void dae::SceneManager::Update()
{
	EventQueue::Instance().SendAll();
	m_CurrentScene->Start();
	m_CurrentScene->Update();
	CollisionQueue::Instance().CheckAndSend();
}
void dae::SceneManager::Render()
{
	m_CurrentScene->Render();
}
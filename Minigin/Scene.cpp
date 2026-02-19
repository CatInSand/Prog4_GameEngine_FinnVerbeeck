#include <algorithm>
#include "Scene.h"
#include "Text.h"

void dae::Scene::Add(std::unique_ptr<GameObject>&& object)
{
	assert(object != nullptr && "Cannot add a null GameObject to the scene.");
	m_Objects.emplace_back(std::move(object));
}

void dae::Scene::RemoveAll()
{
	m_Objects.clear();
}

void dae::Scene::Update()
{
	for(auto& object : m_Objects)
	{
		object->Update();
	}

	DeleteQueue();
}

void dae::Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		object->Render();
	}
}

void dae::Scene::DeleteQueue()
{
	//delete objects
	m_Objects.erase(std::remove_if(m_Objects.begin(), m_Objects.end(),
		[](std::unique_ptr<dae::GameObject>& object) { return object->IsMarkedForDeletion(); }),
		m_Objects.end());

	//delete components from remaining objects
	for (std::unique_ptr<dae::GameObject>& object : m_Objects)
	{
		object->DeleteQueue();
	}
}
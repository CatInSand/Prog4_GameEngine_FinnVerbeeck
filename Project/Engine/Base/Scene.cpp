#include <algorithm>
#include "Scene.h"

dae::Scene::Scene(const std::string& name)
	: m_Name{ name }
{
}

void dae::Scene::Add(std::unique_ptr<GameObject>&& object)
{
	assert(object != nullptr && "Cannot add a null GameObject to the scene.");
	m_Objects.emplace_back(std::move(object));
	m_PriorityListDirty = true;
}

void dae::Scene::RemoveAll()
{
	m_Objects.clear();
	m_PriorityListDirty = true;
}

dae::GameObject* dae::Scene::GetObjectWithName(const cat::hash_t& name)
{
	for (std::unique_ptr<GameObject>& pObject : m_Objects)
	{
		if (pObject->Name() == name)
		{
			return pObject.get();
		}
	}
	return nullptr;
}
dae::GameObject* dae::Scene::GetObjectWithTag(const cat::hash_t& tag)
{
	for (std::unique_ptr<GameObject>& pObject : m_Objects)
	{
		if (pObject->HasTag(tag))
		{
			return pObject.get();
		}
	}
	return nullptr;
}
std::vector<dae::GameObject*> dae::Scene::GetAllObjectsWithName(const cat::hash_t& name)
{
	std::vector<dae::GameObject*> result{};
	for (std::unique_ptr<GameObject>& pObject : m_Objects)
	{
		if (pObject->Name() == name)
		{
			result.push_back(pObject.get());
		}
	}
	return result;
}
std::vector<dae::GameObject*> dae::Scene::GetAllObjectsWithTag(const cat::hash_t& tag)
{
	std::vector<dae::GameObject*> result{};
	for (std::unique_ptr<GameObject>& pObject : m_Objects)
	{
		if (pObject->HasTag(tag))
		{
			result.push_back(pObject.get());
		}
	}
	return result;
}

void dae::Scene::Start()
{
	if (m_NewObjectIndex == m_Objects.size())
		return;
	else if (m_NewObjectIndex > m_Objects.size())
		throw std::runtime_error("NewObjectIndex became invalid");
	else
	{
		while (m_NewObjectIndex < m_Objects.size())
		{
			m_Objects[m_NewObjectIndex]->Start();
			++m_NewObjectIndex;
		}
	}
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
	CalculatePriorityList();

	for (const auto& [index, priority] : m_PriorityIndexList)
	{
		m_Objects[index]->Render();
	}

	/*for (const auto& object : m_Objects)
	{
		object->Render();
	}*/
}

void dae::Scene::DeleteQueue()
{
	//delete objects
	size_t deletedCount{ static_cast<size_t>(std::count_if(m_Objects.begin(), m_Objects.end(),
			[](std::unique_ptr<dae::GameObject>& object) { return object->IsMarkedForDeletion(); }))
	};
	if (deletedCount > 0)
	{
		m_NewObjectIndex -= deletedCount;
		m_PriorityListDirty = true;

		m_Objects.erase(std::remove_if(m_Objects.begin(), m_Objects.end(),
			[](std::unique_ptr<dae::GameObject>& object) { return object->IsMarkedForDeletion(); }),
			m_Objects.end());
	}

	//delete components from remaining objects
	for (std::unique_ptr<dae::GameObject>& object : m_Objects)
	{
		object->DeleteQueue();
	}
}

dae::GameObject* dae::Scene::Root() const
{
	return m_RootObject.get();
}
std::string dae::Scene::Name() const
{
	return m_Name;
}

void dae::Scene::CalculatePriorityList() const
{
	if (m_PriorityListDirty)
	{
		m_PriorityIndexList.clear();
		m_PriorityIndexList.reserve(m_Objects.size());

		size_t index{ 0 };
		for (const auto& object : m_Objects)
		{
			m_PriorityIndexList.push_back({ index, object->RenderPriority() });
			++index;
		}

		std::sort(m_PriorityIndexList.begin(), m_PriorityIndexList.end(),
			[](const std::pair<size_t, int>& val1, const std::pair<size_t, int>& val2)
			{
				return val1.second > val2.second;
			}
		);

		m_PriorityListDirty = false;
	}
}
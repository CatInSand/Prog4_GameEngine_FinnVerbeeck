#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

#include <algorithm>
#include "DeltaTime.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update()
{
	for (std::unique_ptr<dae::Component>& pComponent : m_pComponents)
	{
		pComponent->Update();
	}
}

void dae::GameObject::Render() const
{
	if (m_texture != nullptr)
	{
		const auto& pos = m_transform.GetPosition();
		Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
	}

}

void dae::GameObject::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}

void dae::GameObject::AddComponent(std::unique_ptr<dae::Component>&& pComponent)
{
	m_pComponents.push_back(std::move(pComponent));
}
void dae::GameObject::RemoveComponent(const std::string& id)
{
	//
	//ADD DELETE QUEUE!!!
	//
	m_pComponents.erase(std::remove_if(m_pComponents.begin(), m_pComponents.end(),
		[id](const std::unique_ptr<dae::Component>& component) { return component->m_ID == id; }
	));
}
std::unique_ptr<dae::Component>& dae::GameObject::GetComponent(const std::string& id)
{
	auto it = std::find_if(m_pComponents.begin(), m_pComponents.end(),
		[id](const std::unique_ptr<dae::Component>& component) { return component->m_ID == id; }
	);

	assert(it == m_pComponents.end());
	return *it;
}
bool dae::GameObject::HasComponent(const std::string& id)
{
	const auto it = std::find_if(m_pComponents.cbegin(), m_pComponents.cend(),
		[id](const std::unique_ptr<dae::Component>& component) { return component->m_ID == id; }
		);

	if (it != m_pComponents.cend())
	{
		return true;
	}
	else
	{
		return false;
	}
}
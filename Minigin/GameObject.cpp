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

	for (const std::unique_ptr<dae::Component>& pComponent : m_pComponents)
	{
		pComponent->Render();
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
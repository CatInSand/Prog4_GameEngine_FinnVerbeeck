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
	if (m_Texture != nullptr)
	{
		const auto& pos = m_Transform.GetPosition();
		Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
	}

	for (const std::unique_ptr<dae::Component>& pComponent : m_pComponents)
	{
		pComponent->Render();
	}
}

void dae::GameObject::SetTexture(const std::string& filename)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}

void dae::GameObject::Delete()
{
	m_MarkedForDeletion = true;
}
bool dae::GameObject::IsMarkedForDeletion() const
{
	return m_MarkedForDeletion;
}
void dae::GameObject::DeleteQueue()
{
	m_pComponents.erase(std::remove_if(m_pComponents.begin(), m_pComponents.end(),
		[](std::unique_ptr<dae::Component>& component) { return component->IsMarkedForDeletion(); }),
		m_pComponents.end());
}
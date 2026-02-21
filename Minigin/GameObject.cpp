#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

#include <algorithm>
#include "DeltaTime.h"
#include "RenderComponent.h"

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
	for (const std::unique_ptr<dae::Component>& pComponent : m_pComponents)
	{
		dae::RenderComponent* pRenderComponent{ dynamic_cast<dae::RenderComponent*>(pComponent.get()) };
		if (pRenderComponent != nullptr)
		{
			pRenderComponent->Render();
		}
	}
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
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

dae::Transform dae::GameObject::GetTransform() const
{
	return m_Transform;
}
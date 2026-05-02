#include "RenderComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GameObject.h"

dae::RenderComponent::RenderComponent(dae::GameObject* owner)
	: Component(owner)
{
}

dae::RenderComponent::RenderComponent(dae::GameObject* owner, const std::string& filename)
	: Component(owner)
	, m_pTexture{ ResourceManager::GetInstance().LoadTexture(filename) }
{
}

void dae::RenderComponent::Render()
{
	if (m_pTexture != nullptr)
	{
		const auto& pos = GetOwner()->GetWorldTransform().GetPosition();
		Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
	}
}
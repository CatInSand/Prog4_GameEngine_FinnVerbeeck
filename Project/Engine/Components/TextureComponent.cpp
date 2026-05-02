#include "TextureComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GameObject.h"

dae::TextureComponent::TextureComponent(dae::GameObject* owner)
	: RenderComponent(owner)
{
}

dae::TextureComponent::TextureComponent(dae::GameObject* owner, const std::string& filename)
	: RenderComponent(owner)
	, m_pTexture{ ResourceManager::GetInstance().LoadTexture(filename) }
{
}

void dae::TextureComponent::Render()
{
	if (m_pTexture != nullptr)
	{
		const auto& pos = GetOwner()->GetWorldTransform().GetPosition();
		Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
	}
}
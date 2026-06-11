#include "TextureComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Texture2D.h"

dae::TextureComponent::TextureComponent(dae::GameObject* owner)
	: RenderComponent(owner)
{
}

dae::TextureComponent::TextureComponent(dae::GameObject* owner, const std::string& filename)
	: RenderComponent(owner)
	, m_pTexture{ ResourceManager::Instance().LoadTexture(filename) }
{
}

void dae::TextureComponent::SetTexture(const std::string& filename)
{
	auto newpTexture{ ResourceManager::Instance().LoadTexture(filename) };
	m_pTexture.swap(newpTexture);
}
glm::vec2 dae::TextureComponent::GetSize() const
{
	return m_pTexture->GetSize();
}

void dae::TextureComponent::Render()
{
	if (m_pTexture != nullptr)
	{
		const glm::vec2 pos{ GetOwner()->GetWorldTransform().position };
		Renderer::Instance().RenderTexture(*m_pTexture, pos.x, pos.y);
	}
}
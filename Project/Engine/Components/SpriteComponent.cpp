#include "SpriteComponent.h"
#include "ResourceManager.h"
#include "DeltaTime.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Texture2D.h"

dae::SpriteComponent::SpriteComponent(GameObject* owner, const std::string& filename, Type type, float timePerFrame)
	: RenderComponent(owner, filename)
	, m_TimePerFrame{ timePerFrame }
	, m_SingleSize{ static_cast<uint16_t>(m_pTexture->GetSize().y), static_cast<uint16_t>(m_pTexture->GetSize().y) }
	, m_FrameCount{ static_cast<uint8_t>(m_pTexture->GetSize().x / m_SingleSize.x) }
	, m_Type{ type }
{
}
dae::SpriteComponent::SpriteComponent(GameObject* owner, const std::string& filename, Type type, float timePerFrame, Size size)
	: RenderComponent(owner, filename)
	, m_TimePerFrame{ timePerFrame }
	, m_SingleSize{ size }
	, m_FrameCount{ static_cast<uint8_t>(m_pTexture->GetSize().x / m_SingleSize.x) }
	, m_Type{ type }
{
}
dae::SpriteComponent::SpriteComponent(GameObject* owner, const std::string& filename, Type type, float timePerFrame, uint8_t frameCount)
	: RenderComponent(owner, filename)
	, m_TimePerFrame{ timePerFrame }
	, m_SingleSize{ static_cast<uint16_t>(static_cast<uint32_t>(m_pTexture->GetSize().x) / frameCount), static_cast<uint16_t>(m_pTexture->GetSize().y) }
	, m_FrameCount{ frameCount }
	, m_Type{ type }
{
}
dae::SpriteComponent::SpriteComponent(GameObject* owner, const std::string& filename, Type type, float timePerFrame, Size size, uint8_t frameCount)
	: RenderComponent(owner, filename)
	, m_TimePerFrame{ timePerFrame }
	, m_SingleSize{ size }
	, m_FrameCount{ frameCount }
	, m_Type{ type }
{
}

void dae::SpriteComponent::Update()
{
	m_FrameTimer += gDeltaTime;

	switch (m_Type)
	{
	case dae::SpriteComponent::Type::loop:
		while (m_FrameTimer >= m_TimePerFrame)
		{
			m_FrameTimer -= m_TimePerFrame;
			m_CurrentFrame = (m_CurrentFrame + 1) % m_FrameCount;
		}
		break;
	case dae::SpriteComponent::Type::swing:
		while (m_FrameTimer >= m_TimePerFrame)
		{
			m_FrameTimer -= m_TimePerFrame;
			m_CurrentFrame = (m_CurrentFrame + 1) % (m_FrameCount * 2 - 2);
		}
		break;
	default:
		break;
	}
}
void dae::SpriteComponent::Render()
{
	if (m_pTexture != nullptr)
	{
		const glm::vec2 pos{ GetOwner()->GetWorldTransform().GetPosition() };
		float srcX{};
		Rect srcRect{};

		switch (m_Type)
		{
		case dae::SpriteComponent::Type::single:
			Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
			break;
		case dae::SpriteComponent::Type::loop:
			srcRect = {
				.left = static_cast<float>(m_CurrentFrame * m_SingleSize.x),
				.top = 0,
				.width = static_cast<float>(m_SingleSize.x),
				.height = static_cast<float>(m_SingleSize.y),
			};
			Renderer::GetInstance().RenderTextureCut(*m_pTexture, pos.x, pos.y, srcRect.left, srcRect.top, srcRect.width, srcRect.height);
			break;
		case dae::SpriteComponent::Type::swing:
			if (m_CurrentFrame < m_FrameCount)
			{
				srcX = static_cast<float>(m_CurrentFrame * m_SingleSize.x);
			}
			else
			{
				srcX = static_cast<float>((2 * m_FrameCount - 2 - m_CurrentFrame) * m_SingleSize.x);
			}
			srcRect = {
				.left = srcX,
				.top = 0,
				.width = static_cast<float>(m_SingleSize.x),
				.height = static_cast<float>(m_SingleSize.y),
			};
			Renderer::GetInstance().RenderTextureCut(*m_pTexture, pos.x, pos.y, srcRect.left, srcRect.top, srcRect.width, srcRect.height);
			break;
		default:
			break;
		}
	}
}
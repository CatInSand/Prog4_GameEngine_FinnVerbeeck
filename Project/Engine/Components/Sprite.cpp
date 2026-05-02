#include "Sprite.h"
#include "ResourceManager.h"
#include "DeltaTime.h"
#include "Renderer.h"
#include "Texture2D.h"

dae::Sprite::Sprite(const std::string& filename, Type type, float timePerFrame)
	: m_pTexture{ ResourceManager::GetInstance().LoadTexture(filename) }
	, m_TimePerFrame{ timePerFrame }
	, m_SingleSize{ static_cast<uint16_t>(m_pTexture->GetSize().y), static_cast<uint16_t>(m_pTexture->GetSize().y) }
	, m_FrameCount{ static_cast<uint8_t>(m_pTexture->GetSize().x / m_pTexture->GetSize().y) }
	, m_Type{ type }
{
}
dae::Sprite::Sprite(const std::string& filename, Type type, float timePerFrame, glm::vec2 textureSize, uint8_t frameCount)
	: m_pTexture{ ResourceManager::GetInstance().LoadTexture(filename) }
	, m_TimePerFrame{ timePerFrame }
	, m_SingleSize{ static_cast<uint16_t>(textureSize.y), static_cast<uint16_t>(textureSize.y) }
	, m_FrameCount{ frameCount }
	, m_Type{ type }
{
}

void dae::Sprite::Update()
{
	m_FrameTimer += gDeltaTime;

	switch (m_Type)
	{
	case dae::Sprite::Type::loop:
		while (m_FrameTimer >= m_TimePerFrame)
		{
			m_FrameTimer -= m_TimePerFrame;
			m_CurrentFrame = (m_CurrentFrame + 1) % m_FrameCount;
		}
		break;
	case dae::Sprite::Type::swing:
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
void dae::Sprite::Render(const glm::vec2& pos)
{
	float srcX{};
	Rect srcRect{};

	switch (m_Type)
	{
	case dae::Sprite::Type::single:
		Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
		break;
	case dae::Sprite::Type::loop:
		srcRect = {
			.left = static_cast<float>(m_CurrentFrame * m_SingleSize.x),
			.top = 0,
			.width = static_cast<float>(m_SingleSize.x),
			.height = static_cast<float>(m_SingleSize.y),
		};
		Renderer::GetInstance().RenderTextureCut(*m_pTexture, pos.x, pos.y, srcRect.left, srcRect.top, srcRect.width, srcRect.height);
		break;
	case dae::Sprite::Type::swing:
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
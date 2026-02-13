#include "Text.h"

#include <stdexcept>
#include <SDL3_ttf/SDL_ttf.h>
#include "Font.h"
#include "Texture2D.h"

dae::Text::Text(const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color)
	: m_Text{ text }
	, m_Color{ color }
	, m_pFont{ std::move(font) }
{
}

void dae::Text::Update()
{
	if (m_NeedsUpdate)
	{
		const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), m_Text.length(), m_Color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_DestroySurface(surf);
		m_pTextTexture = std::make_shared<Texture2D>(texture);
		m_NeedsUpdate = false;
	}
}

void dae::Text::Render() const
{
	if (m_pTextTexture != nullptr)
	{
		const auto& pos = m_Transform.GetPosition();
		Renderer::GetInstance().RenderTexture(*m_pTextTexture, pos.x, pos.y);
	}
}

void dae::Text::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void dae::Text::SetPosition(const float x, const float y)
{
	m_Transform.SetPosition(x, y);
}

void dae::Text::SetColor(const SDL_Color& color)
{
	m_Color = color;
	m_NeedsUpdate = true;
}
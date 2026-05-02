#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <memory>
#include "Transform.h"
#include "Renderer.h"

#include "RenderComponent.h"

namespace dae
{
	class Font;
	class Texture2D;
	class GameObject;
	class TextComponent : public RenderComponent
	{
	public:
		TextComponent(GameObject*, const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color = { 255, 255, 255, 255 });
		virtual ~TextComponent() = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

		virtual void Update() override;

		virtual void SetText(const std::string& text);
		void SetPosition(float x, float y);
		void SetColor(const SDL_Color& color);

	protected:
		bool m_NeedsUpdate{ true };
		std::string m_Text;
		SDL_Color m_Color;
		std::shared_ptr<Font> m_pFont;
	};
}

#endif
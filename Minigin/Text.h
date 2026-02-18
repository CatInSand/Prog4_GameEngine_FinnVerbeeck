#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <memory>
#include "Transform.h"
#include "Renderer.h"

#include "Component.h"

namespace dae
{
	class Font;
	class Texture2D;
	class GameObject;
	class Text : public Component
	{
	public:
		Text(std::unique_ptr<GameObject>& owner, const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color = { 255, 255, 255, 255 });
		virtual ~Text() = default;
		Text(const Text& other) = delete;
		Text(Text&& other) = delete;
		Text& operator=(const Text& other) = delete;
		Text& operator=(Text&& other) = delete;

		virtual void Update() override;
		virtual void Render() const override;

		virtual void SetText(const std::string& text);
		void SetPosition(float x, float y);
		void SetColor(const SDL_Color& color);

	protected:
		bool m_NeedsUpdate{ true };
		std::string m_Text;
		SDL_Color m_Color;
		Transform m_Transform{};
		std::shared_ptr<Font> m_pFont;
		std::shared_ptr<Texture2D> m_pTextTexture{ nullptr };
	};
}

#endif
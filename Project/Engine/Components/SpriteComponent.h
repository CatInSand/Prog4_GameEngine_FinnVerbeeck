#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include <memory>

#include "RenderComponent.h"

namespace dae
{
	struct Size
	{
		uint16_t x;
		uint16_t y;
	};
	struct Rect
	{
		float left;
		float top;
		float width;
		float height;
	};

	class SpriteComponent final : public RenderComponent
	{
	public:
		enum class Type : uint8_t {
			single,
			loop,
			swing,
		};

		SpriteComponent(GameObject* owner, const std::string& filename, Type type, float timePerFrame = 1.f);
		SpriteComponent(GameObject* owner, const std::string& filename, Type type, float timePerFrame, Size size);
		SpriteComponent(GameObject* owner, const std::string& filename, Type type, float timePerFrame, uint8_t frameCount);
		SpriteComponent(GameObject* owner, const std::string& filename, Type type, float timePerFrame, Size size, uint8_t frameCount);

		virtual void Update() override;
		virtual void Render() override;

	private:
		float m_TimePerFrame;
		float m_FrameTimer{ 0.f };
		Size m_SingleSize;
		uint8_t m_FrameCount;
		uint8_t m_CurrentFrame{ 0 };
		Type m_Type;
	};
}

#endif

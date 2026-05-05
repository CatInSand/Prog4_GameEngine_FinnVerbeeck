#ifndef SPRITE_H
#define SPRITE_H

#include <memory>
#include <string>
#include <glm/glm.hpp>

namespace dae
{
	class Texture2D;
	class Sprite final
	{
	public:
		enum class Type : uint8_t {
			still,
			single,
			loop,
			swing,
		};

		Sprite(const std::string& filename, Type type, float timePerFrame = 1.f);
		Sprite(const std::string& filename, Type type, float timePerFrame, glm::vec2 textureSize, uint8_t frameCount);

		void Update();
		void Render(const glm::vec2& position);

	private:
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

		std::shared_ptr<Texture2D> m_pTexture;
		float m_TimePerFrame;
		float m_FrameTimer{ 0.f };
		Size m_SingleSize;
		uint8_t m_FrameCount;
		uint8_t m_CurrentFrame{ 0 };
		Type m_Type;
	};
}

#endif
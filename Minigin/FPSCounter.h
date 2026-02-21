#ifndef FPS_COUNTER_H
#define FPS_COUNTER_H

#include "Text.h"
#include "DeltaTime.h"
#include <vector>

namespace dae
{
	class GameObject;
	class FPSCounter final : public Text
	{
	public:
		FPSCounter(GameObject* owner, const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color = { 255, 255, 255, 255 });

		virtual void Update() override;
		float GetFPS() const;

	private:
		float m_FPS{ gFrameRate };
		std::vector<float> m_Framerates{};
		const uint8_t m_FrameCount{ 10 };
	};
}

#endif
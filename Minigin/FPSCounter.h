#ifndef FPS_COUNTER_H
#define FPS_COUNTER_H

#include "Text.h"
#include "DeltaTime.h"

namespace dae
{
	class GameObject;
	class FPSCounter final : public Text
	{
	public:
		FPSCounter(std::unique_ptr<GameObject>& owner, const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color = { 255, 255, 255, 255 });

		virtual void Update() override;
		float GetFPS() const;

		virtual std::string GetID() override { return m_ID; };

	private:
		const std::string m_ID{ "FPSCounter" };

		float m_FPS{ gFrameRate };
	};
}

#endif
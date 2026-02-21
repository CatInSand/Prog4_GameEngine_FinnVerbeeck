#ifndef FPS_COUNTER_H
#define FPS_COUNTER_H

#include "Text.h"
#include "DeltaTime.h"
#include <vector>

namespace dae
{
	class GameObject;
	class FPSCounter final : public Component
	{
	public:
		FPSCounter(GameObject* owner);

		virtual void Update() override;
		float GetFPS() const;

	private:
		float m_FPS{ gFrameRate };
		std::vector<float> m_Framerates{};
		const uint8_t m_FrameCount{ 10 };
	};
}

#endif
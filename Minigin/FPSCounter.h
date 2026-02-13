#ifndef FPS_COUNTER_H
#define FPS_COUNTER_H

#include "Component.h"
#include "DeltaTime.h"

namespace dae
{
	class FPSCounter final : public Component
	{
	public:
		FPSCounter() = default;

		virtual void Update() override;
		float GetFPS() const;

	private:
		float m_FPS{ gFrameRate };
	};
}

#endif
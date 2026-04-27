#ifndef FPS_COMPONENT_H
#define FPS_COMPONENT_H

#include "Component.h"

namespace dae
{
	class GameObject;
	class FPSCounter;
	class Text;
	class FPSComponent final : public Component
	{
	public:
		FPSComponent(GameObject* owner);
		virtual ~FPSComponent() = default;
		FPSComponent(const FPSComponent& other) = delete;
		FPSComponent(FPSComponent&& other) = delete;
		FPSComponent& operator=(const FPSComponent& other) = delete;
		FPSComponent& operator=(FPSComponent&& other) = delete;

		virtual void Update() override;

	private:
		FPSCounter* m_pFPSCounterComponent{ nullptr };
		Text* m_pTextComponent{ nullptr };
	};
}

#endif
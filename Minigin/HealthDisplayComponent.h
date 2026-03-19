#ifndef HEALTH_DISPLAY_COMPONENT_H
#define HEALTH_DISPLAY_COMPONENT_H

#include "Component.h"

namespace dae
{
	class HealthDisplayComponent final : public Component
	{
	public:
		HealthDisplayComponent(GameObject* owner);

		virtual void Update() override;

	private:

	};
}

#endif
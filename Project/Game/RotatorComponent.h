#ifndef ROTATOR_COMPONENT_H
#define ROTATOR_COMPONENT_H

#include "Component.h"
#include <glm/glm.hpp>

namespace dae
{
	class GameObject;
	class RotatorComponent final : public Component
	{
	public:
		RotatorComponent(GameObject* owner, float speed);
		virtual ~RotatorComponent() = default;
		RotatorComponent(const RotatorComponent& other) = delete;
		RotatorComponent(RotatorComponent&& other) = delete;
		RotatorComponent& operator=(const RotatorComponent& other) = delete;
		RotatorComponent& operator=(RotatorComponent&& other) = delete;

		virtual void Update() override;

	private:
		void Initialize();

		float m_Speed;
		float m_CurrentAngle{};
		float m_Distance{};
	};
}

#endif
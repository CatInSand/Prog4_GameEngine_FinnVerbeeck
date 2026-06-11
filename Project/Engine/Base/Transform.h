#pragma once
#include <glm/glm.hpp>

namespace dae
{
	struct Transform final
	{
		Transform(glm::vec2 position = {0.f, 0.f}, glm::vec2 scale = {1.f, 1.f});

		void Move(glm::vec2 deltaPos);
		void Move(float deltaX, float deltaY);

		Transform operator*(const Transform& other);

		glm::vec2 position;
		glm::vec2 scale;
	};
}

#include "Transform.h"


dae::Transform::Transform(glm::vec2 _position, glm::vec2 _scale)
	: position{ _position }
	, scale{ _scale }
{
}

void dae::Transform::Move(glm::vec2 deltaPos)
{
	position += deltaPos;
}
void dae::Transform::Move(float deltaX, float deltaY)
{
	position.x += deltaX;
	position.y += deltaY;
}

dae::Transform dae::Transform::operator*(const Transform& other)
{
	return Transform {
		position + other.position,
		{ scale.x * other.scale.x, scale.y * other.scale.y }
	};
}
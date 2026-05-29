#include "Transform.h"


dae::Transform::Transform(glm::vec2 _position, float _rotation, glm::vec2 _scale)
	: position{ _position }
	, rotation{ _rotation }
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
void dae::Transform::Rotate(float deltaAngle)
{
	rotation += deltaAngle;
}
//void dae::Transform::Scale(glm::vec2 deltaScale)
//{
//	m_Scale.x *= deltaScale.x;
//	m_Scale.y *= deltaScale.y;
//}

dae::Transform dae::Transform::operator*(const Transform& other)
{
	return Transform {
		position + other.position,
		rotation + other.rotation,
		{ scale.x * other.scale.x, scale.y * other.scale.y }
	};
}
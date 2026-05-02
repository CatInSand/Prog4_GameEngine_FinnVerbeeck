#include "SpriteComponent.h"
#include "GameObject.h"

dae::SpriteComponent::SpriteComponent(GameObject* owner, Sprite&& sprite)
	: RenderComponent(owner)
	, m_Sprite{ std::move(sprite) }
{
}
void dae::SpriteComponent::Update()
{
	m_Sprite.Update();
}
void dae::SpriteComponent::Render()
{
	m_Sprite.Render(GetOwner()->GetWorldTransform().GetPosition());
}
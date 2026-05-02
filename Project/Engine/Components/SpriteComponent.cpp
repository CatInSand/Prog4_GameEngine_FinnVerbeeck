#include "SpriteComponent.h"
#include "ResourceManager.h"
#include "DeltaTime.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Texture2D.h"

dae::SpriteComponent::SpriteComponent(GameObject* owner, Sprite&& sprite)
	: TextureComponent(owner)
	, m_Sprite{ std::move(sprite) }
{
}
void dae::SpriteComponent::Update()
{
	m_Sprite.Update();
}
void dae::SpriteComponent::Render()
{
	m_Sprite.Update();
}
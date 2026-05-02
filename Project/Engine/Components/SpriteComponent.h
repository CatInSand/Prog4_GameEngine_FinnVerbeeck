#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include <memory>

#include "RenderComponent.h"
#include "Sprite.h"

namespace dae
{
	class SpriteComponent final : public RenderComponent
	{
	public:
		SpriteComponent(GameObject* owner, Sprite&& sprite);

		virtual void Update() override;
		virtual void Render() override;

	private:
		Sprite m_Sprite;
	};
}

#endif

#ifndef MULTI_SPRITE_COMPONENT_H
#define MULTI_SPRITE_COMPONENT_H

#include <unordered_map>
#include "Sprite.h"
#include "RenderComponent.h"
#include "GameObject.h"

namespace dae
{
	using sprite_id = uint8_t;

	class MultiSpriteComponent : public RenderComponent
	{
	public:
		MultiSpriteComponent(GameObject* owner, std::unordered_map<sprite_id, Sprite>&& spriteMap, sprite_id defaultSpriteIndex = 0)
			: RenderComponent(owner)
			, m_SpriteMap{ std::move(spriteMap) }
			, m_CurrentSprite{ m_SpriteMap.at(defaultSpriteIndex) }
		{
		}

		virtual void Update() override
		{
			m_CurrentSprite.Update();
		}
		virtual void Render() override
		{
			m_CurrentSprite.Render(GetOwner()->GetWorldTransform().GetPosition());
		}

		void SetSprite(sprite_id spriteID)
		{
			m_CurrentSprite = m_SpriteMap.at(spriteID);
		}

	private:
		std::unordered_map<sprite_id, dae::Sprite> m_SpriteMap;
		dae::Sprite& m_CurrentSprite;
	};
}

#endif

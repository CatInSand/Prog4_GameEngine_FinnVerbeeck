#ifndef TEXTURE_COMPONENT_H
#define TEXTURE_COMPONENT_H

#include "RenderComponent.h"

namespace dae
{
	class Texture2D;
	class TextureComponent : public RenderComponent
	{
	public:
		TextureComponent(GameObject* owner);
		TextureComponent(GameObject* owner, const std::string& filename);
		virtual ~TextureComponent() = default;
		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent(TextureComponent&& other) = delete;
		TextureComponent& operator=(const TextureComponent& other) = delete;
		TextureComponent& operator=(TextureComponent&& other) = delete;

		virtual void Update() override
		{
		}
		virtual void Render() override;

	protected:
		std::shared_ptr<Texture2D> m_pTexture{ nullptr };
	};
}

#endif
#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include "Component.h"
#include <string>

namespace dae
{
	class Texture2D;
	class TextureComponent : public Component
	{
	public:
		TextureComponent(GameObject* owner);
		TextureComponent(GameObject* owner, const std::string& filename);
		virtual ~TextureComponent() = default;
		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent(TextureComponent&& other) = delete;
		TextureComponent& operator=(const TextureComponent& other) = delete;
		TextureComponent& operator=(TextureComponent&& other) = delete;

		virtual void Update() override {}
		virtual void Render();

	protected:
		std::shared_ptr<Texture2D> m_pTexture{ nullptr };
	};
}

#endif
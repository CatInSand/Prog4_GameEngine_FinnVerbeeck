#ifndef TEXTURE_COMPONENT_H
#define TEXTURE_COMPONENT_H

#include "RenderComponent.h"
#include "Texture2D.h"

namespace dae
{
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

		void SetTexture(const std::string& filename);
		glm::vec2 GetSize() const;

		virtual void Update() override
		{
		}
		virtual void Render() override;

	protected:
		std::shared_ptr<Texture2D> m_pTexture{ nullptr };
	};
}

#endif
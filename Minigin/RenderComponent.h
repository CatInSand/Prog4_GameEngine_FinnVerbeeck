#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include "Component.h"

namespace dae
{
	class Texture2D;
	class RenderComponent : public Component
	{
	public:
		RenderComponent(GameObject* owner);
		virtual ~RenderComponent() = default;
		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;

		virtual void Update() override {}
		virtual void Render() const;
		void SetTexture(const std::string& filename);

	protected:
		std::shared_ptr<Texture2D> m_pTexture{ nullptr };
	};
}

#endif
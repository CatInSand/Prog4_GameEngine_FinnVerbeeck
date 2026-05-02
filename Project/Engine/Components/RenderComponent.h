#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include "Component.h"

namespace dae
{
	class RenderComponent : public Component
	{
	public:
		virtual ~RenderComponent() = default;
		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;

		virtual void Update() = 0;
		virtual void Render() = 0;

	protected:
		explicit RenderComponent(GameObject* owner)
			: Component(owner)
		{
		}
	};
}

#endif

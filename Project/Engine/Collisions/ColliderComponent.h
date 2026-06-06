#ifndef COLLIDER_COMPONENT_H
#define COLLIDER_COMPONENT_H

#include "Observer.h"
#include "Component.h"
#include "CollisionQueue.h"

namespace dae
{
	class ColliderComponent : public Observer, public Component
	{
	public:
		ColliderComponent(GameObject* pOwner, Transform* colliderTransform);
		virtual ~ColliderComponent();
		ColliderComponent(const ColliderComponent& other) = delete;
		ColliderComponent(ColliderComponent&& other) = delete;
		ColliderComponent& operator=(const ColliderComponent& other) = delete;
		ColliderComponent& operator=(ColliderComponent&& other) = delete;

		virtual void Update() = 0;
		virtual void Notify(std::unique_ptr<Event>& pEvent) override;

	protected:
		virtual void OnCollisionEnter(GameObject* pObject) = 0;
		virtual void OnCollisionStay(GameObject* pObject) = 0;
		virtual void OnCollisionExit(GameObject* pObject) = 0;

	private:
		BoxCollider m_Collider;
	};
}

#endif

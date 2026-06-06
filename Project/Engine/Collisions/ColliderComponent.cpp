#include "ColliderComponent.h"

dae::ColliderComponent::ColliderComponent(GameObject* pOwner, Transform* colliderTransform)
	: Component(pOwner)
	, m_Collider{ colliderTransform, pOwner }
{
	CollisionQueue::Instance().AddCollider(this, &m_Collider);
}
dae::ColliderComponent::~ColliderComponent()
{
	CollisionQueue::Instance().RemoveCollider(this, &m_Collider);
}

void dae::ColliderComponent::Notify(std::unique_ptr<Event>& pEvent)
{
	if (pEvent->m_ID == "Collision"_h)
	{
		CollisionEvent* currentEvent{ reinterpret_cast<CollisionEvent*>(pEvent.get()) };

		switch (currentEvent->m_Type)
		{
		case CollisionEvent::Type::enter:
			OnCollisionEnter(currentEvent->m_pObject);
			break;
		case CollisionEvent::Type::stay:
			OnCollisionStay(currentEvent->m_pObject);
			break;
		case CollisionEvent::Type::exit:
			OnCollisionExit(currentEvent->m_pObject);
			break;
		default:
			break;
		}
	}
}
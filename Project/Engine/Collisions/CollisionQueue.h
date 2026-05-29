#ifndef COLLISION_QUEUE_H
#define COLLISION_QUEUE_H

#include <memory>
#include <vector>

#include "Subject.h"
#include "Collider.h"
#include "Singleton.h"

namespace dae
{
	class CollisionEvent final : public Event
	{
	public:
		enum class Type : uint8_t {
			enter,
			stay,
			exit,
			none,
		};

		CollisionEvent(Type type, GameObject* pObject)
			: Event("Collision"_h)
			, m_pObject{ pObject }
			, m_Type{ type }
		{}
		GameObject* m_pObject;
		Type m_Type;
	};

	class CollisionQueue final : public _BaseSubject, public Singleton<CollisionQueue>
	{
	public:
		virtual ~CollisionQueue() = default;

		void AddCollider(Observer* pObserver, BoxCollider* pCollider);
		void RemoveCollider(Observer* pObserver, BoxCollider* pCollider);

		void CheckAndSend();

	private:
		struct CollisionData
		{
			Observer* pObserver1;
			BoxCollider* pCollider1;
			Observer* pObserver2;
			BoxCollider* pCollider2;
			CollisionEvent::Type type;
		};

		void CheckForCollisions();
		void SendAll();
		void AddCollisions(CollisionEvent::Type type, size_t indexFirst, size_t indexSecond);

		std::vector<BoxCollider*> m_Colliders{};

		std::vector<CollisionData> m_CurrentCollisions{};
		std::vector<CollisionData> m_PreviousCollisions{};
	};
}

#endif

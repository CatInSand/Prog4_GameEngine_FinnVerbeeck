#include "CollisionQueue.h"

void dae::CollisionQueue::AddCollider(Observer* pObserver, BoxCollider* pCollider)
{
	AddObserver(pObserver);
	m_Colliders.push_back(pCollider);
}
void dae::CollisionQueue::RemoveCollider(Observer* pObserver, BoxCollider* pCollider)
{
	RemoveObserver(pObserver);
	m_Colliders.erase(
		std::remove(m_Colliders.begin(), m_Colliders.end(), pCollider),
		m_Colliders.end()
	);
}

void dae::CollisionQueue::CheckAndSend()
{
	CheckForCollisions();
	SendAll();
}

void dae::CollisionQueue::CheckForCollisions()
{
	if (m_Colliders.size() < 2)
		return;

	for (size_t firstCollIndex{ 0 }; firstCollIndex < m_Colliders.size() - 1; ++firstCollIndex)
	{
		for (size_t secondCollIndex{ firstCollIndex + 1 }; secondCollIndex < m_Colliders.size(); ++secondCollIndex)
		{
			auto it{ std::find_if(m_PreviousCollisions.begin(), m_PreviousCollisions.end(),
				[&](const CollisionData& collision)
				{
					return collision.pCollider1 == m_Colliders[firstCollIndex] && collision.pCollider2 == m_Colliders[secondCollIndex];
				})
			};

			CollisionEvent::Type previousType{
				it != m_PreviousCollisions.end() ? it->type : CollisionEvent::Type::none
			};

			CollisionEvent::Type currentType{
				[&]() {
					if (BoxCollider::Collide(*m_Colliders[firstCollIndex], *m_Colliders[secondCollIndex]))
					{
						switch (previousType)
						{
						case dae::CollisionEvent::Type::enter:
						case dae::CollisionEvent::Type::stay:
							return dae::CollisionEvent::Type::stay;
						case dae::CollisionEvent::Type::exit:
						case dae::CollisionEvent::Type::none:
							return dae::CollisionEvent::Type::enter;
						default:
							break;
						}
					}
					else
					{
						switch (previousType)
						{
						case dae::CollisionEvent::Type::enter:
						case dae::CollisionEvent::Type::stay:
							return dae::CollisionEvent::Type::exit;
						case dae::CollisionEvent::Type::exit:
						case dae::CollisionEvent::Type::none:
							return dae::CollisionEvent::Type::none;
						default:
							break;
						}
					}

					assert(false && "Invalid CollisionEvent::Type");
					return dae::CollisionEvent::Type::none;
				}()
			};

			if (currentType != CollisionEvent::Type::none)
			{
				AddCollisions(currentType, firstCollIndex, secondCollIndex);
			}
		}
	}
}

void dae::CollisionQueue::SendAll()
{
	for (CollisionData& collision : m_CurrentCollisions)
	{
		std::unique_ptr<Event> event1{ std::make_unique<CollisionEvent>(
			collision.type,
			collision.pCollider2->Owner())
		};
		collision.pObserver1->Notify(event1);

		std::unique_ptr<Event> event2{ std::make_unique<CollisionEvent>(
			collision.type,
			collision.pCollider1->Owner())
		};
		collision.pObserver2->Notify(event1);
	}

	m_PreviousCollisions = std::move(m_CurrentCollisions);
	m_CurrentCollisions.clear();
}

void dae::CollisionQueue::AddCollisions(CollisionEvent::Type type, size_t indexFirst, size_t indexSecond)
{
	m_CurrentCollisions.push_back(
		CollisionData{
			.pObserver1 = m_Observers[indexFirst],
			.pCollider1 = m_Colliders[indexFirst],
			.pObserver2 = m_Observers[indexSecond],
			.pCollider2 = m_Colliders[indexSecond],
			.type = type
		}
	);
}
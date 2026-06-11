#ifndef COLLIDER_H
#define COLLIDER_H

#include "Transform.h"
#include "GameObject.h"

namespace dae
{
	class BoxCollider final
	{
	public:
		BoxCollider(Transform* transform, GameObject* pOwner)
			: m_Transform{ transform }
			, m_pOwner{ pOwner }
		{
			assert(m_pOwner != nullptr);
		}
		~BoxCollider() = default;
		BoxCollider(const BoxCollider& other) = delete;
		BoxCollider(BoxCollider&& other) = default;
		BoxCollider& operator=(const BoxCollider& other) = delete;
		BoxCollider& operator=(BoxCollider&& other) = default;

		/*
		Checks if the boxes collide. Ignores rotation.
		*/
		static bool Collide(const BoxCollider& bc1, const BoxCollider& bc2)
		{
			Transform transform1{ bc1.m_pOwner->GetWorldTransform() };
			Transform transform2{ bc2.m_pOwner->GetWorldTransform() };

			if (transform1.position.x < transform2.position.x + transform2.scale.x
				&& transform1.position.x + transform1.scale.x > transform2.position.x
				&& transform1.position.y < transform2.position.y + transform2.scale.y
				&& transform1.position.y + transform1.scale.y > transform2.position.y)
			{
				return true;
			}

			return false;
		}
		GameObject* Owner()
		{
			return m_pOwner;
		}

	private:
		Transform* m_Transform;
		GameObject* m_pOwner;
	};
}

#endif
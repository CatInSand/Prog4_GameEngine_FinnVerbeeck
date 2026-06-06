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
			if (bc1.m_Transform->position.x < bc2.m_Transform->position.x + bc2.m_Transform->scale.x
				&& bc1.m_Transform->position.x + bc1.m_Transform->scale.x > bc2.m_Transform->position.x
				&& bc1.m_Transform->position.y < bc2.m_Transform->position.y + bc2.m_Transform->scale.y
				&& bc1.m_Transform->position.y + bc1.m_Transform->scale.y > bc2.m_Transform->position.y)
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
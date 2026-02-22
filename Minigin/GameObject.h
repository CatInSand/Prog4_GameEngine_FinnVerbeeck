#pragma once
#include <string>
#include <memory>
#include "Transform.h"

#include "Component.h"
#include <vector>

namespace dae
{
	class GameObject final
	{
	public:
		GameObject() = default;	//should take GameObject* pParent
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		template<typename T>
		void AddComponent(std::unique_ptr<dae::Component>&& pComponent)
		{
			if (!HasComponent<T>())
			{
				m_pComponents.push_back(std::move(pComponent));
			}
			else
			{
				throw std::exception("Attempted to add duplicate component");
			}
		}
		template<typename T>
		void DeleteComponent()
		{
			for (std::unique_ptr<dae::Component>& pComponent : m_pComponents)
			{
				if (dynamic_cast<T*>(pComponent.get()) != nullptr)
				{
					pComponent->Delete();
				}
			}
		}
		template<typename T>
		T* GetComponent()
		{
			auto it = std::find_if(m_pComponents.begin(), m_pComponents.end(),
				[](const std::unique_ptr<dae::Component>& pComponent) { return dynamic_cast<T*>(pComponent.get()) != nullptr; }
			);

			assert(it != m_pComponents.end());
			return dynamic_cast<T*>((*it).get());
		}
		template<typename T>
		bool HasComponent()
		{
			const auto it = std::find_if(m_pComponents.cbegin(), m_pComponents.cend(),
				[](const std::unique_ptr<dae::Component>& pComponent) { return dynamic_cast<const T*>(pComponent.get()) != nullptr; }
			);

			if (it != m_pComponents.cend())
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		void Update();
		void Render() const;

		void Delete();
		bool IsMarkedForDeletion() const;
		void DeleteQueue();

		void SetLocalPosition(float x, float y);
		Transform GetLocalTransform() const;
		Transform GetWorldTransform() const;

		void SetParent(GameObject* pParent, bool keepWorldTransform = true);
		const GameObject* GetParent() const;
		size_t GetChildCount() const;
		const GameObject* GetChildAtIndex(unsigned int index) const;

	private:
		void AddChild(GameObject* pChild);
		void RemoveChild(GameObject* pChild);
		bool IsChild(GameObject* pChild);

		bool m_MarkedForDeletion{ false };

		void CalculateWorldTransform();

		Transform m_WorldTransform{};
		Transform m_LocalTransform{};
		bool m_TransformRequiresUpdate{ true };	//might need to be false initially

		std::vector<std::unique_ptr<Component>> m_pComponents{};

		GameObject* m_pParent;
		std::vector<GameObject*> m_pChildren{};
	};
}

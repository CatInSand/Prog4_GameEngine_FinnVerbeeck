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
		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Delete();
		bool IsMarkedForDeletion() const;
		void DeleteQueue();

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

		void SetPosition(float x, float y);

		Transform GetTransform() const;

	private:
		bool m_MarkedForDeletion{ false };
		Transform m_Transform{};

		std::vector<std::unique_ptr<Component>> m_pComponents{};
	};
}

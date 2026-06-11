#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <string>
#include <memory>
#include <vector>
#include <stdexcept>

#include <cat/hash.h>
using namespace cat::hash_literals; //add literal overload _h

#include "Transform.h"
#include "Component.h"


namespace dae
{
	class GameObject final
	{
	public:
		GameObject(GameObject* pParent, const std::string& name);
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;
		
		//Name & tags
		cat::hash_t Name() const;
		void Rename(const cat::hash_t& name);
		bool HasTag(const cat::hash_t& tag) const;
		void AddTag(const cat::hash_t& tag);
		void RemoveTag(const cat::hash_t& tag);
		std::vector<cat::hash_t> Tags() const;

		//Components
		template<typename T>
		void AddComponent(std::unique_ptr<dae::Component>&& pComponent)
		{
			if (!HasComponent<T>())
			{
				m_pComponents.push_back(std::move(pComponent));
			}
			else
			{
				throw std::invalid_argument("Attempted to add duplicate component");
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

		//Parent-child
		void SetParent(GameObject* pParent, bool keepWorldTransform = true);
		const GameObject* GetParent() const;
		GameObject* GetRoot();
		const GameObject* GetRoot() const;
		size_t GetChildCount() const;
		GameObject* GetChildAtIndex(size_t index);

		//Gameloop
		void Start();
		void Update();
		void Render();

		//Deletion
		void Delete();
		bool IsMarkedForDeletion() const;
		void DeleteQueue();

		//Transform
		void SetLocalPosition(float x, float y);
		void SetLocalPosition(glm::vec2 pos);
		Transform GetLocalTransform() const;
		Transform GetWorldTransform();
		Transform* LocalTransformPtr();

		//Rendering
		void RenderPrioritize(int priority);
		int RenderPriority() const;

	private:
		//Name & tags
		cat::hash_t m_Name;
		std::vector<cat::hash_t> m_Tags{};

		//Components
		std::vector<std::unique_ptr<Component>> m_pComponents{};

		//Parent-child
		void AddChild(GameObject* pChild);
		void RemoveChild(GameObject* pChild);
		bool IsChild(GameObject* pChild);

		GameObject* m_pParent;
		std::vector<GameObject*> m_pChildren{};

		//Deletion
		bool m_MarkedForDeletion{ false };

		//Transform
		void SetTransformDirty();
		void CalculateWorldTransform();
		
		Transform m_WorldTransform{};
		Transform m_LocalTransform{};
		bool m_TransformRequiresUpdate{ false };

		//Rendering
		int m_RenderPriority{ 0 };
	};
}

#endif

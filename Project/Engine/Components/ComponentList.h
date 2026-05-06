#ifndef COMPONENT_LIST_H
#define COMPONENT_LIST_H

#include <concepts>
#include <vector>
#include <memory>
#include <cassert>
#include "Component.h"
#include "RenderComponent.h"

namespace dae
{
	template<typename T>
		requires requires () { std::derived_from<T, Component>; }
	class ComponentList final : public Component
	{
	public:
		ComponentList(GameObject* pOwner)
			: Component(pOwner)
		{
		}
		virtual ~ComponentList() = default;
		ComponentList(const ComponentList& other) = delete;
		ComponentList(ComponentList&& other) = delete;
		ComponentList& operator=(const ComponentList& other) = delete;
		ComponentList& operator=(ComponentList&& other) = delete;

		virtual void Update() override
		{
			for (std::unique_ptr<T>& pComponent : m_Components)
			{
				pComponent->Update();
			}
		};

		void PushBack(std::unique_ptr<T>&& pComponent)
		{
			m_Components.push_back(pComponent);
		}

		T* Get(size_t index)
		{
			assert(index < m_Components.size());
			return m_Components[index].get();
		}

	private:
		std::vector<std::unique_ptr<T>> m_Components{};
	};

	template<typename T>
		requires requires () { std::derived_from<T, RenderComponent>; }
	class RenderComponentList final : public RenderComponent
	{
	public:
		ComponentList(GameObject* pOwner)
			: RenderComponent(pOwner)
		{
		}
		virtual ~ComponentList() = default;
		ComponentList(const ComponentList& other) = delete;
		ComponentList(ComponentList&& other) = delete;
		ComponentList& operator=(const ComponentList& other) = delete;
		ComponentList& operator=(ComponentList&& other) = delete;

		virtual void Update() override
		{
			for (std::unique_ptr<T>& pComponent : m_Components)
			{
				pComponent->Update();
			}
		};

		virtual void Render() override
		{
			for (std::unique_ptr<T>& pComponent : m_Components)
			{
				pComponent->Render();
			}
		};

		void PushBack(std::unique_ptr<T>&& pComponent)
		{
			m_Components.push_back(pComponent);
		}

		T* Get(size_t index)
		{
			assert(index < m_Components.size());
			return m_Components[index].get();
		}

	private:
		std::vector<std::unique_ptr<T>> m_Components{};
	};
}

#endif
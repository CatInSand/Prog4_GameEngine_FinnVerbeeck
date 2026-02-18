#pragma once
#include <string>
#include <memory>
#include "Transform.h"

#include "Component.h"
#include <vector>

namespace dae
{
	class Texture2D;
	class GameObject final
	{
	public:
		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void AddComponent(std::unique_ptr<Component>&& pComponent);
		template<typename T>
		void RemoveComponent()
		{
			//
			//ADD DELETE QUEUE!!!
			//
			m_pComponents.erase(std::remove_if(m_pComponents.begin(), m_pComponents.end(),
				[](const std::unique_ptr<dae::Component>& component) { return typeid(T) == typeid(*component); }
			));
		}
		template<typename T>
		std::unique_ptr<Component>& GetComponent()
		{
			auto it = std::find_if(m_pComponents.begin(), m_pComponents.end(),
				[](const std::unique_ptr<dae::Component>& component) { return typeid(T) == typeid(*component); }
			);

			assert(it == m_pComponents.end());
			return *it;
		}
		template<typename T>
		bool HasComponent()
		{
			const auto it = std::find_if(m_pComponents.cbegin(), m_pComponents.cend(),
				[](const std::unique_ptr<dae::Component>& component) { return typeid(T) == typeid(*component); }
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

		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);

	private:
		Transform m_transform{};
		std::shared_ptr<Texture2D> m_texture{};

		std::vector<std::unique_ptr<Component>> m_pComponents{};
	};
}
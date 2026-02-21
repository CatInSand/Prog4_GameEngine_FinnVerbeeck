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

		void AddComponent(std::unique_ptr<Component>&& pComponent);
		void RemoveComponent(const std::string& id);
		std::unique_ptr<Component>& GetComponent(const std::string& id);
		bool HasComponent(const std::string& id);

		virtual void Update();
		virtual void Render() const;

		void SetPosition(float x, float y);

		Transform GetTransform() const;

	private:
<<<<<<< Updated upstream
		Transform m_transform{};
		std::shared_ptr<Texture2D> m_texture{};
=======
		bool m_MarkedForDeletion{ false };
		Transform m_Transform{};
>>>>>>> Stashed changes

		std::vector<std::unique_ptr<Component>> m_pComponents{};
	};
}

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
		void RemoveComponent(const std::string& id);
		std::unique_ptr<Component>& GetComponent(const std::string& id);
		bool HasComponent(const std::string& id);

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
#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <vector>
#include <string>
#include "GameObject.h"

namespace dae
{
	class Scene final
	{
	public:
		Scene() = default;

		void Add(std::unique_ptr<GameObject>&& object);
		void RemoveAll();

		GameObject* GetObjectWithName(const std::string& name);
		GameObject* GetObjectWithTag(const std::string& tag);
		std::vector<GameObject*> GetAllObjectsWithName(const std::string& name);
		std::vector<GameObject*> GetAllObjectsWithTag(const std::string& tag);

		void Update();
		void Render() const;

		void DeleteQueue();

		GameObject* Root();

		~Scene() = default;
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private:
		std::unique_ptr<GameObject> m_RootObject{ std::make_unique<GameObject>(nullptr, "Root") };
		std::vector<std::unique_ptr<GameObject>> m_Objects{};
	};
}

#endif
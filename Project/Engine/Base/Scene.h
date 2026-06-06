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
		Scene(const std::string& name);
		~Scene() = default;
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		void Add(std::unique_ptr<GameObject>&& object);
		void RemoveAll();

		GameObject* GetObjectWithName(const cat::hash_t& name);
		GameObject* GetObjectWithTag(const cat::hash_t& tag);
		std::vector<GameObject*> GetAllObjectsWithName(const cat::hash_t& name);
		std::vector<GameObject*> GetAllObjectsWithTag(const cat::hash_t& tag);

		void Update();
		void Render() const;

		void DeleteQueue();

		GameObject* Root() const;
		std::string Name() const;

	private:
		std::string m_Name;
		std::unique_ptr<GameObject> m_RootObject{ std::make_unique<GameObject>(nullptr, "Root") };
		std::vector<std::unique_ptr<GameObject>> m_Objects{};
	};
}

#endif
#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <vector>
#include "GameObject.h"

namespace dae
{
	class Scene final
	{
	public:
		void Add(std::unique_ptr<GameObject>&& object);
		void RemoveAll();

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
		friend class SceneManager;
		explicit Scene() = default;

		std::unique_ptr<GameObject> m_RootObject{ std::make_unique<GameObject>(nullptr, "Root") };
		std::vector<std::unique_ptr<GameObject>> m_Objects{};
	};
}

#endif
#include "EnemySpawner.h"

#include <memory>
#include <iostream>

#include "SceneManager.h"
#include "Enemy.h"

dae::EnemySpawner::EnemySpawner(GameObject* pOwner)
	: Component(pOwner)
{
}

void dae::EnemySpawner::Update()
{
}

dae::GameObject* dae::EnemySpawner::SpawnEnemy(EnemyType enemyType, const glm::vec2& position, GameObject* pOwner)
{
	switch (enemyType)
	{
	case dae::EnemyType::pooka:
		return SpawnPooka(position, pOwner);
	case dae::EnemyType::fygar:
		std::cout << "Fygar not spawnable yet\n";
		return nullptr;
	default:
		break;
	}

	return nullptr;
}

dae::GameObject* dae::EnemySpawner::SpawnPooka(const glm::vec2& position, GameObject* pOwner)
{
	// pooka object
	auto gameObject{ std::make_unique<GameObject>(pOwner, "Pooka") };
	gameObject->SetLocalPosition(position);

	auto pookaComponent{ std::make_unique<PookaComponent>(gameObject.get()) };
	gameObject->AddComponent<PookaComponent>(std::move(pookaComponent));

	auto renderComponent{ std::make_unique<TextureComponent>(gameObject.get(), "sprites/pooka.png") };
	gameObject->SetLocalScale(renderComponent->GetSize());
	gameObject->AddComponent<TextureComponent>(std::move(renderComponent));

	GameObject* result{ gameObject.get() };
	SceneManager::Instance().CurrentScene()->Add(std::move(gameObject));

	return result;
}
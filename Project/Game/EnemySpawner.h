#ifndef ENEMY_SPAWNER_H
#define ENEMY_SPAWNER_H

#include "Component.h"
#include "GameObject.h"

namespace dae
{
	enum class EnemyType {
		pooka,
		fygar,
	};

	class EnemySpawner final : public Component
	{
	public:
		EnemySpawner(GameObject* pOwner);
		virtual ~EnemySpawner() = default;

		virtual void Update() override;

		GameObject* SpawnEnemy(EnemyType enemyType, const glm::vec2& position, GameObject* pOwner);

	private:
		GameObject* SpawnPooka(const glm::vec2& position, GameObject* pOwner);
	};
}

#endif

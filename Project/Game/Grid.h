#ifndef GRID_H
#define GRID_H

#include <array>
#include <cmath>

#include "Component.h"
#include "TextureComponent.h"
#include "SceneManager.h"
#include "Texture2D.h"

#include "EnemySpawner.h"

namespace dae
{
	enum class Direction {
		none = -1,
		up,
		down,
		left,
		right,
		count,
	};

	enum class Cell : uint8_t {
		none,
		full,
		empty,
		ground,
		pooka,
		fygar,
		rock,
		solid,
		sky,
	};

	struct BlockData
	{
		float fullness;
		uint8_t layer;
		Cell cellStartingData;
	};

	class BlockComponent final : public TextureComponent
	{
	public:
		BlockComponent(GameObject* pOwner, const std::string& texturePath, float fullness, bool solid = false);
		virtual ~BlockComponent() = default;

		glm::vec2 Size() const;
		bool Full() const;
		bool Empty() const;
		bool IsSolid() const;

		bool Dig(float amount, Direction direction);

		static constexpr float FULL{ 16.f };
		static constexpr float EMPTY{ 0.f };
		static constexpr float EPSILON{ 1.f };

	private:
		float m_Fullness;
		Direction m_Direction{ Direction::none };
		bool m_Solid;
	};
	template<size_t Width, size_t Height>
	struct GridData
	{
		std::array<std::array<BlockData, Width>, Height> grid;
	};

	class GridComponent final : public Component
	{
	public:
		static constexpr int GRID_WIDTH{ 16 };
		static constexpr int GRID_HEIGHT{ 18 };

		GridComponent(GameObject* pOwner);
		virtual ~GridComponent() = default;

		virtual void Update() override;

		glm::vec2 BlockSize() const;
		glm::vec2 PlayerSpawn() const;
		int GetLayer(const glm::vec2& position) const;

		GameObject* CurrentBlock(const glm::vec2& position, Direction direction);
		GameObject* NextBlock(const glm::vec2& position, Direction direction);
		glm::vec2 SnapToGrid(const glm::vec2& position) const;
		glm::vec2 SnapToGridLine(const glm::vec2& position, Direction direction);
		bool CanDigInDir(const glm::vec2& position, Direction direction, Direction previousDirection);
		bool CanMoveInDir(const glm::vec2& position, Direction direction);
		glm::vec2 MoveInDir(const glm::vec2& position, Direction direction, float amount);
		glm::vec2 DigInDir(const glm::vec2& position, Direction direction, Direction previousDirection, float amount);

		void LoadNextLevel();
		void RestartLevel();

	private:
		void LoadLevel(int level);
		GameObject* MakeGridBlock(GameObject* pOwner, glm::vec2 columnRow, BlockData blockData);
		GameObject* SpawnObjects(GameObject* pOwner, glm::vec2 columnRow, BlockData blockData);
		static GridData<GRID_WIDTH, GRID_HEIGHT> LoadGridFromFile(const std::string& filePath);

		std::array<std::array<GameObject*, GRID_WIDTH>, GRID_HEIGHT> m_Grid{};
		glm::vec2 m_BlockSize{};
		EnemySpawner* m_pEnemySpawner{ nullptr };
		int m_CurrentLevel{};
	};
}

#endif

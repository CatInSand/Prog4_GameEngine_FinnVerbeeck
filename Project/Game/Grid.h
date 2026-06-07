#ifndef GRID_H
#define GRID_H

#include <array>
#include <cmath>

#include "Component.h"
#include "TextureComponent.h"
#include "SceneManager.h"
#include "Texture2D.h"

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

	class BlockComponent final : public TextureComponent
	{
	public:
		BlockComponent(GameObject* pOwner, const std::string& fileName)
			: TextureComponent(pOwner, fileName)
		{
			m_Enabled = false;
		}
		virtual ~BlockComponent() = default;

		glm::vec2 Size() const
		{
			return m_pTexture->GetSize();
		}
		bool Empty()
		{
			if (m_Fullness <= EMPTY)
				return true;

			return false;
		}

		bool Dig(float amount, Direction direction)
		{
			if (Empty())
				return false;

			if (m_Direction == Direction::none)
				m_Direction = direction;
			else if (m_Direction != direction)
				return false;
			
			m_Fullness = std::min(FULL - amount, m_Fullness);
			if (Empty())
			{
				m_Fullness = EMPTY;
				return true;
			}

			return false;
		}

	private:
		static constexpr float FULL{ 16.f };
		static constexpr float EMPTY{ 0.f };
		float m_Fullness{ FULL };
		Direction m_Direction{ Direction::none };
	};

	GameObject* MakeGridBlock(GameObject* pOwner, glm::vec2 columnRow, const std::string& fileName);

	class GridComponent final : public Component
	{
	public:
		static constexpr int GRID_WIDTH{ 10 };
		static constexpr int GRID_HEIGHT{ 16 };

		GridComponent(GameObject* pOwner);
		virtual ~GridComponent() = default;

		virtual void Update() override;

		glm::vec2 BlockSize() const;

		GameObject* CurrentBlock(const glm::vec2& position);
		GameObject* NextBlock(const glm::vec2& position, Direction direction);
		glm::vec2 SnapToGrid(const glm::vec2& position);
		glm::vec2 SnapToGridLine(const glm::vec2& position, Direction direction);
		bool CanDigInDir(const glm::vec2& position, Direction direction);
		bool CanMoveInDir(const glm::vec2& position, Direction direction);
		glm::vec2 MoveInDir(const glm::vec2& position, Direction direction, float amount);
		glm::vec2 DigInDir(const glm::vec2& position, Direction direction, float amount);

	private:
		std::array<std::array<GameObject*, GRID_WIDTH>, GRID_HEIGHT> m_Grid{};
		glm::vec2 m_BlockSize{};
	};
}

#endif

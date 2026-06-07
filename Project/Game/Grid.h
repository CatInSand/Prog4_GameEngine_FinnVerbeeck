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
		none,
		up,
		down,
		left,
		right,
	};

	class BlockComponent final : public TextureComponent
	{
	public:
		BlockComponent(GameObject* pOwner, const std::string& fileName)
			: TextureComponent(pOwner, fileName)
		{}
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

	GameObject* MakeGridBlock(GameObject* pOwner, glm::vec2 columnRow, const std::string& fileName)
	{
		std::unique_ptr<dae::GameObject> gameObject{ std::make_unique<dae::GameObject>(pOwner, "GridBlock") };

		std::unique_ptr<dae::BlockComponent> blockComponent{ std::make_unique<dae::BlockComponent>(gameObject.get(), fileName)};
		gameObject->SetLocalPosition(columnRow.x * blockComponent->Size().x, columnRow.y * blockComponent->Size().y);
		gameObject->AddComponent<dae::BlockComponent>(std::move(blockComponent));

		GameObject* result{ gameObject.get() };
		SceneManager::Instance().CurrentScene()->Add(std::move(gameObject));

		return result;
	}

	class GridComponent final : public Component
	{
	public:
		static constexpr int GRID_WIDTH{ 10 };
		static constexpr int GRID_HEIGHT{ 16 };

		GridComponent(GameObject* pOwner)
			: Component(pOwner)
		{
			int row{ 0 };
			int column{ 0 };
			for (std::array<GameObject*, GRID_WIDTH>& arr : m_Grid)
			{
				column = 0;
				for (GameObject*& element : arr)
				{
					std::string fileName{ [=]() 
						{
							if (row < 4)
								return "sprites/pooka_walk.png";
							else if (row < 8)
								return "sprites/pooka_walk.png";
							else if (row < 12)
								return "sprites/pooka_walk.png";
							else if (row < 16)
								return "sprites/pooka_walk.png";
							
							return "";
						}()
					};

					element = MakeGridBlock(pOwner, { column , row }, fileName);
					++column;
				}
				++row;
			}

			m_BlockSize = m_Grid[0][0]->GetComponent<BlockComponent>()->Size();
		}
		virtual ~GridComponent() = default;

		virtual void Update() override
		{
		}

		glm::vec2 BlockSize() const
		{
			return m_BlockSize;
		}

		GameObject* CurrentBlock(const glm::vec2& position)
		{
			size_t row{ static_cast<size_t>(position.x / BlockSize().y) };
			size_t column{ static_cast<size_t>(position.y / BlockSize().x) };
			return m_Grid[row][column];
		}
		GameObject* NextBlock(const glm::vec2& position, Direction direction)
		{
			size_t row{ static_cast<size_t>(position.x / BlockSize().y) };
			size_t column{ static_cast<size_t>(position.y / BlockSize().x) };

			switch (direction)
			{
			case dae::Direction::down:
				++row;
				break;
			case dae::Direction::right:
				++column;
				break;
			default:
				break;
			}

			return m_Grid[row][column];
		}
		glm::vec2 SnapToGrid(const glm::vec2& position)
		{
			return { std::fmodf(position.x, BlockSize().x),  std::fmodf(position.y, BlockSize().y) };
		}
		glm::vec2 SnapToGridLine(const glm::vec2& position, Direction direction)
		{
			switch (direction)
			{
			case dae::Direction::up:
			case dae::Direction::down:
				return { std::fmodf(position.x, BlockSize().x), position.y };
				break;
			case dae::Direction::left:
			case dae::Direction::right:
				return { position.x, std::fmodf(position.y, BlockSize().y) };
				break;
			default:
				assert(false && "invalid direction");
				return position;
				break;
			}
		}
		bool CanDigInDir(const glm::vec2& position, Direction direction)
		{
			return position == SnapToGridLine(position, direction);
		}
		bool CanMoveInDir(const glm::vec2& position, Direction direction)
		{
			return CanDigInDir(position, direction) && NextBlock(position, direction)->GetComponent<BlockComponent>()->Empty();
		}
		glm::vec2 MoveInDir(const glm::vec2& position, Direction direction, float amount)
		{
			if (CanMoveInDir(position, direction))
			{
				glm::vec2 newPos{ position };
				switch (direction)
				{
				case dae::Direction::up:
					newPos.y -= amount;
					break;
				case dae::Direction::down:
					newPos.y += amount;
					break;
				case dae::Direction::left:
					newPos.x -= amount;
					break;
				case dae::Direction::right:
					newPos.x += amount;
					break;
				default:
					break;
				}

				return newPos;
			}
			else
			{
				return position;
			}
		}
		glm::vec2 DigInDir(const glm::vec2& position, Direction direction, float amount)
		{
			if (CanDigInDir(position, direction))
			{
				GameObject* pBlock{ NextBlock(position, direction) };

				if(pBlock->GetComponent<BlockComponent>()->Dig(amount, direction))
				{
					switch (direction)
					{
					case dae::Direction::up:
						return SnapToGrid({ position.x, position.y - amount });
					case dae::Direction::down:
						return SnapToGrid({ position.x, position.y + amount });
					case dae::Direction::left:
						return SnapToGrid({ position.x - amount, position.y });
					case dae::Direction::right:
						return SnapToGrid({ position.x + amount, position.y });
						break;
					default:
						return position;
						break;
					}
				}
			}
			else
			{
				return position;
			}
		}

	private:
		std::array<std::array<GameObject*, GRID_WIDTH>, GRID_HEIGHT> m_Grid{};
		glm::vec2 m_BlockSize{};
	};
}

#endif

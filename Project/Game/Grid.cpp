#include "Grid.h"

dae::GameObject* dae::MakeGridBlock(GameObject* pOwner, glm::vec2 columnRow, const std::string& fileName)
{
	std::unique_ptr<dae::GameObject> gameObject{ std::make_unique<dae::GameObject>(pOwner, "GridBlock") };

	std::unique_ptr<dae::BlockComponent> blockComponent{ std::make_unique<dae::BlockComponent>(gameObject.get(), fileName) };
	gameObject->SetLocalPosition(columnRow.x * blockComponent->Size().x, columnRow.y * blockComponent->Size().y);
	gameObject->AddComponent<dae::BlockComponent>(std::move(blockComponent));

	GameObject* result{ gameObject.get() };
	SceneManager::Instance().CurrentScene()->Add(std::move(gameObject));

	return result;
}

dae::GridComponent::GridComponent(GameObject* pOwner)
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

void dae::GridComponent::Update()
{
}

glm::vec2 dae::GridComponent::BlockSize() const
{
	return m_BlockSize;
}

dae::GameObject* dae::GridComponent::CurrentBlock(const glm::vec2& position)
{
	size_t row{ static_cast<size_t>(position.x / BlockSize().y) };
	size_t column{ static_cast<size_t>(position.y / BlockSize().x) };
	return m_Grid[row][column];
}
dae::GameObject* dae::GridComponent::NextBlock(const glm::vec2& position, Direction direction)
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
glm::vec2 dae::GridComponent::SnapToGrid(const glm::vec2& position)
{
	return { std::fmodf(position.x, BlockSize().x),  std::fmodf(position.y, BlockSize().y) };
}
glm::vec2 dae::GridComponent::SnapToGridLine(const glm::vec2& position, Direction direction)
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
bool dae::GridComponent::CanDigInDir(const glm::vec2& position, Direction direction)
{
	return position == SnapToGridLine(position, direction);
}
bool dae::GridComponent::CanMoveInDir(const glm::vec2& position, Direction direction)
{
	return CanDigInDir(position, direction) && NextBlock(position, direction)->GetComponent<BlockComponent>()->Empty();
}
glm::vec2 dae::GridComponent::MoveInDir(const glm::vec2& position, Direction direction, float amount)
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
glm::vec2 dae::GridComponent::DigInDir(const glm::vec2& position, Direction direction, float amount)
{
	if (CanDigInDir(position, direction))
	{
		GameObject* pBlock{ NextBlock(position, direction) };

		if (pBlock->GetComponent<BlockComponent>()->Dig(amount, direction))
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
				break;
			}
		}

		return position;
	}
	else
	{
		return position;
	}
}
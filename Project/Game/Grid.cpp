#include "Grid.h"

#include <fstream>

#include "ResourceManager.h"

dae::BlockComponent::BlockComponent(GameObject* pOwner, const std::string& texturePath, float fullness)
	: TextureComponent(pOwner, texturePath)
	, m_Fullness{ fullness }
{
}

glm::vec2 dae::BlockComponent::Size() const
{
	return m_pTexture->GetSize();
}
bool dae::BlockComponent::Full() const
{
	if (m_Fullness == FULL)
		return true;

	return false;
}
bool dae::BlockComponent::Empty() const
{
	if (m_Fullness <= EMPTY)
		return true;

	return false;
}

bool dae::BlockComponent::Dig(float amount, Direction direction)
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

dae::GameObject* dae::MakeGridBlock(GameObject* pOwner, glm::vec2 columnRow, BlockData blockData)
{
	std::string texturePath{};
	switch (blockData.cellStartingData)
	{
	case dae::Cell::rock:
	case dae::Cell::full:
		texturePath = std::format("sprites/tile_layer{}.png", static_cast<int>(blockData.layer));
		break;
	case dae::Cell::pooka:
	case dae::Cell::fygar:
	case dae::Cell::empty:
		texturePath = "sprites/tile_empty.png";
		break;
	case dae::Cell::solid:
		texturePath = "sprites/tile_empty.png";
		break;
	case dae::Cell::ground:
	case dae::Cell::sky:
		texturePath = "sprites/tile_sky.png";
		break;
	default:
		break;
	}

	auto gameObject{ std::make_unique<dae::GameObject>(pOwner, "GridBlock") };

	auto blockComponent{ std::make_unique<dae::BlockComponent>(gameObject.get(), texturePath, blockData.fullness)};
	gameObject->SetLocalPosition(columnRow.x * blockComponent->Size().x, columnRow.y * blockComponent->Size().y);
	gameObject->AddComponent<dae::BlockComponent>(std::move(blockComponent));

	GameObject* result{ gameObject.get() };
	SceneManager::Instance().CurrentScene()->Add(std::move(gameObject));

	return result;
}

dae::GridComponent::GridComponent(GameObject* pOwner)
	: Component(pOwner)
{
	auto grid{ LoadGridFromFile("levels/level1.txt") };

	int row{ 0 };
	int column{ 0 };
	for (std::array<GameObject*, GRID_WIDTH>& arr : m_Grid)
	{
		column = 0;
		for (GameObject*& element : arr)
		{
			element = MakeGridBlock(pOwner, { column , row }, grid.grid[row][column]);
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

dae::GridData<dae::GridComponent::GRID_WIDTH, dae::GridComponent::GRID_HEIGHT> dae::GridComponent::LoadGridFromFile(const std::string& filePath)
{
	GridData<GRID_WIDTH, GRID_HEIGHT> result{};

	if (std::ifstream fileStream{ ResourceManager::Instance().DataPath() / filePath }; fileStream.good())
	{
		char nextChar{};
		BlockData blockData{};

		for (int row{}; row < GRID_HEIGHT; ++row)
		{
			for (int column{}; column < GRID_WIDTH; ++column)
			{
				fileStream.get(nextChar);

				switch (nextChar)
				{
				case 'R': //rock
					blockData.cellStartingData = Cell::rock;
					blockData.fullness = BlockComponent::FULL;
					break;
				case '#': //filled tile
					blockData.cellStartingData = Cell::full;
					blockData.fullness = BlockComponent::FULL;
					break;
				case '.': //empty tile
					blockData.cellStartingData = Cell::empty;
					blockData.fullness = BlockComponent::EMPTY;
					break;
				case '0': //surface
					blockData.cellStartingData = Cell::ground;
					blockData.fullness = BlockComponent::EMPTY;
					break;
				case 'P': //pooka
					blockData.cellStartingData = Cell::pooka;
					blockData.fullness = BlockComponent::EMPTY;
					break;
				case 'F': //fygar
					blockData.cellStartingData = Cell::fygar;
					blockData.fullness = BlockComponent::EMPTY;
					break;
				case 'S': //sky
					blockData.cellStartingData = Cell::sky;
					blockData.fullness = BlockComponent::FULL;
					break;
				case 'B': //bottom
					blockData.cellStartingData = Cell::solid;
					blockData.fullness = BlockComponent::FULL;
					break;
				default:
					assert(false && "error loading file, couldn't match data with tile");
					blockData.cellStartingData = Cell::none;
					break;
				}

				blockData.layer = static_cast<uint8_t>((row - 1) / 4);

				result.grid[row][column] = blockData;
			}
			fileStream.ignore(INT_MAX, '\n');
		}
	}

	return result;
}
#include "Grid.h"

#include <fstream>
#include <iostream>

#include "ResourceManager.h"

dae::BlockComponent::BlockComponent(GameObject* pOwner, const std::string& texturePath, float fullness, bool solid)
	: TextureComponent(pOwner, texturePath)
	, m_Fullness{ fullness }
	, m_Solid{ solid }
{
}

glm::vec2 dae::BlockComponent::Size() const
{
	return m_pTexture->GetSize();
}
bool dae::BlockComponent::Full() const
{
	if (m_Fullness >= FULL - EPSILON)
		return true;

	return false;
}
bool dae::BlockComponent::Empty() const
{
	if (m_Fullness <= EMPTY + EPSILON)
		return true;

	return false;
}
bool dae::BlockComponent::IsSolid() const
{
	return m_Solid;
}

bool dae::BlockComponent::Dig(float amount, Direction direction)
{
	if (Empty())
		return false;

	if (m_Direction == Direction::none)
		m_Direction = direction;

	m_Fullness -= amount;
	if (Empty())
	{
		SetTexture("sprites/tile_empty.png");
		return true;
	}

	return false;
}

dae::GameObject* dae::GridComponent::MakeGridBlock(GameObject* pOwner, glm::vec2 columnRow, BlockData blockData)
{
	std::string texturePath{};
	bool solid{ false };
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
		solid = true;
		break;
	case dae::Cell::ground:
		texturePath = "sprites/tile_sky.png";
		break;
	case dae::Cell::sky:
		texturePath = "sprites/tile_sky.png";
		solid = true;
		break;
	default:
		break;
	}

	auto gameObject{ std::make_unique<dae::GameObject>(pOwner, "GridBlock") };

	auto blockComponent{ std::make_unique<dae::BlockComponent>(gameObject.get(), texturePath, blockData.fullness, solid)};
	gameObject->SetLocalPosition(columnRow.x * blockComponent->Size().x, columnRow.y * blockComponent->Size().y);
	gameObject->AddComponent<dae::BlockComponent>(std::move(blockComponent));

	GameObject* result{ gameObject.get() };
	SceneManager::Instance().CurrentScene()->Add(std::move(gameObject));

	return result;
}
dae::GameObject* dae::GridComponent::SpawnObjects(GameObject* pOwner, glm::vec2 columnRow, BlockData blockData)
{
	switch (blockData.cellStartingData)
	{
	case dae::Cell::pooka:
		return m_pEnemySpawner->SpawnEnemy(EnemyType::pooka, { columnRow.x * BlockSize().x, columnRow.y * BlockSize().y }, pOwner);
	default:
		break;
	}

	return nullptr;
}

dae::GridComponent::GridComponent(GameObject* pOwner)
	: Component(pOwner)
{
	m_pEnemySpawner = GetOwner()->GetComponent<EnemySpawner>();

	LoadLevel(1);
}

void dae::GridComponent::LoadNextLevel()
{
	LoadLevel(m_CurrentLevel + 1);
}

void dae::GridComponent::LoadLevel(int level)
{
	if (level > 3)
		return;

	m_CurrentLevel = level;

	size_t childCount{ GetOwner()->GetChildCount() };
	for (size_t childIndex{}; childIndex < childCount; ++childIndex)
	{
		GetOwner()->GetChildAtIndex(0)->Delete();
	}

	auto grid{ LoadGridFromFile(std::format("levels/level{}.txt", level))};

	int row{ 0 };
	int column{ 0 };
	for (std::array<GameObject*, GRID_WIDTH>& arr : m_Grid)
	{
		column = 0;
		for (GameObject*& element : arr)
		{
			element = MakeGridBlock(GetOwner(), {column , row}, grid.grid[row][column]);
			++column;
		}
		++row;
	}

	m_BlockSize = m_Grid[0][0]->GetComponent<BlockComponent>()->Size();

	row = 0;
	column = 0;
	for (std::array<GameObject*, GRID_WIDTH>& arr : m_Grid)
	{
		column = 0;
		for (GameObject*& element : arr)
		{
			(void)element; // can't just remove it :(
			SpawnObjects(GetOwner(), { column , row }, grid.grid[row][column]);
			++column;
		}
		++row;
	}
}

void dae::GridComponent::Update()
{
}

glm::vec2 dae::GridComponent::BlockSize() const
{
	return m_BlockSize;
}
glm::vec2 dae::GridComponent::PlayerSpawn() const
{
	constexpr uint8_t column{ 7 };
	constexpr uint8_t row{ 9 };
	return { m_BlockSize.x * column, m_BlockSize.y * row };
}

dae::GameObject* dae::GridComponent::CurrentBlock(const glm::vec2& position, Direction direction)
{
	size_t row{ static_cast<size_t>(SnapToGrid(position).y / BlockSize().y) };
	size_t column{ static_cast<size_t>(SnapToGrid(position).x / BlockSize().x) };

	if (position != SnapToGrid(position))
	{
		switch (direction)
		{
		case dae::Direction::up:
			++row;
			break;
		case dae::Direction::left:
			++column;
			break;
		default:
			break;
		}
	}

	return m_Grid[row][column];
}
dae::GameObject* dae::GridComponent::NextBlock(const glm::vec2& position, Direction direction)
{
	size_t row{ static_cast<size_t>(SnapToGrid(position).y / BlockSize().y) };
	size_t column{ static_cast<size_t>(SnapToGrid(position).x / BlockSize().x) };

	if (position != SnapToGrid(position))
	{
		switch (direction)
		{
		case dae::Direction::up:
			++row;
			break;
		case dae::Direction::left:
			++column;
			break;
		default:
			break;
		}
	}

	switch (direction)
	{
	case Direction::up:
		return m_Grid[row - 1][column];
	case Direction::down:
		return m_Grid[row + 1][column];
	case Direction::left:
		return m_Grid[row][column - 1];
	case Direction::right:
		return m_Grid[row][column + 1];
	default:
		break;
	}

	return m_Grid[row][column];
}
glm::vec2 dae::GridComponent::SnapToGrid(const glm::vec2& position)
{
	return { floorf(position.x / BlockSize().x) * BlockSize().x,  floorf(position.y / BlockSize().y) * BlockSize().y };
}
glm::vec2 dae::GridComponent::SnapToGridLine(const glm::vec2& position, Direction direction)
{
	switch (direction)
	{
	case dae::Direction::up:
	case dae::Direction::down:
		return { floorf(position.x / BlockSize().x) * BlockSize().x, position.y };
		break;
	case dae::Direction::left:
	case dae::Direction::right:
		return { position.x,  floorf(position.y / BlockSize().y) * BlockSize().y };
		break;
	default:
		assert(false && "invalid direction");
		return position;
		break;
	}
}
bool dae::GridComponent::CanDigInDir(const glm::vec2& position, Direction direction, Direction previousDirection)
{
	bool canDig{};

	canDig = abs(glm::distance(position, SnapToGridLine(position, direction))) < 2.f
		&& !NextBlock(position, direction)->GetComponent<BlockComponent>()->IsSolid()
		&& (NextBlock(position, previousDirection)->GetComponent<BlockComponent>()->Empty()
		|| NextBlock(position, previousDirection)->GetComponent<BlockComponent>()->Full()
		|| direction == previousDirection);

	return canDig;
}
bool dae::GridComponent::CanMoveInDir(const glm::vec2& position, Direction direction)
{
	return abs(glm::distance(position, SnapToGridLine(position, direction))) < 2.f
		&& !NextBlock(position, direction)->GetComponent<BlockComponent>()->IsSolid()
		&& NextBlock(position, direction)->GetComponent<BlockComponent>()->Empty();
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

		if (NextBlock(position, direction) == CurrentBlock(newPos, direction))
		{
			switch (direction)
			{
			case dae::Direction::up:
			case dae::Direction::left:
				SnapToGrid(position);
				break;
			case dae::Direction::down:
			case dae::Direction::right:
				SnapToGrid(newPos);
				break;
			default:
				break;
			}
		}

		return newPos;
	}
	else
	{
		return position;
	}
}
glm::vec2 dae::GridComponent::DigInDir(const glm::vec2& position, Direction direction, Direction previousDirection, float amount)
{
	if (CanDigInDir(position, direction, previousDirection))
	{
		GameObject* pBlock{ NextBlock(position, direction) };

		glm::vec2 newPos{ SnapToGridLine(position, direction) };

		switch (direction)
		{
		case dae::Direction::up:
			newPos = { newPos.x, newPos.y - amount };
			break;
		case dae::Direction::down:
			newPos = { position.x, position.y + amount };
			break;
		case dae::Direction::left:
			newPos = { newPos.x - amount, newPos.y };
			break;
		case dae::Direction::right:
			newPos = { newPos.x + amount, newPos.y };
			break;
		default:
			break;
		}

		if (pBlock->GetComponent<BlockComponent>()->Dig(amount, direction))
		{
			newPos = SnapToGrid(pBlock->GetLocalTransform().position);
		}

		return newPos;
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
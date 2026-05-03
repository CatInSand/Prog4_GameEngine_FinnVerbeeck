#include "GridComponent.h"

//GridBlock
static constexpr uint8_t EMPTY_SIZE{ 0 };
static constexpr uint8_t FULL_SIZE{ 16 };

dae::GridBlock::GridBlock(bool filled)
	: m_Fullness{ filled ? FULL_SIZE : EMPTY_SIZE }
{
}
bool dae::GridBlock::Empty()
{
	return m_Fullness == EMPTY_SIZE;
}
bool dae::GridBlock::Full()
{
	return m_Fullness == FULL_SIZE;
}

//GridComponent
dae::GridComponent::GridComponent(GameObject* owner)
	: Component(owner)
	, m_Grid{ CreateGrid(0) }
{
}

void dae::GridComponent::Update()
{
}

dae::GridComponent::grid_array dae::GridComponent::CreateGrid(uint8_t)
{
	/*
	             f
	DDDDDD DDDDDDD
	DDDDDD DDDDDDD
	D DDDD DD    D
	D DDrD DDDDDDD
	D DDDD DDDDDDD
	D DDDD DDDDDDD
	DDDDDD DDDDDDD
	DDDDD d DDDDDD
	DDDDDDDDDDDDDD
	DDDDDDDDD rDDD
	DD    DDD DDDD
	DDDrDDDDD DDDD
	DDDDDDDDD DDDD
	DDDDDDDDD DDDD
	DDDDDDDDDDDDDD
	*/

	grid_array result{};

	GridBlock filledBlock{ true };
	GridBlock emptyBlock{ false };

	//fill grid except sky
	{
		RectI8 skyRect{
		.left = 0,
		.top = 1,
		.width = WIDTH,
		.height = HEIGHT - 1
		};
		SetRect(result, skyRect, filledBlock);
	}

	//empty middle hole
	{
		constexpr int holeDepth{ 7 };
		RectI8 holeRect{
			.left = WIDTH / 2 - 2,
			.top = 1,
			.width = 1,
			.height = holeDepth - 1
		};
		SetRect(result, holeRect, emptyBlock);
		RectI8 holeBottomRect{
			.left = WIDTH / 2 - 3,
			.top = holeDepth,
			.width = 3,
			.height = 1
		};
		SetRect(result, holeBottomRect, emptyBlock);
	}

	return result;
}

void dae::GridComponent::SetRect(grid_array& grid, RectI8 rect, GridBlock block)
{
	for (uint8_t column{ rect.left }; column < rect.width + rect.left; ++column)
	{
		for (uint8_t row{ rect.top }; row < rect.height + rect.top; ++row)
		{
			grid[column][row] = block;
		}
	}
}
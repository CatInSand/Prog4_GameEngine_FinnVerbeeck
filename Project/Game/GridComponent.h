#ifndef GRID_COMPONENT_H
#define GRID_COMPONENT_H

#include <array>

#include "Component.h"

namespace dae
{
	struct GridBlock final
	{
		enum class Direction : uint8_t {
			none,
			up,
			right,
			down,
			left
		};
		
		GridBlock(bool filled = false);

		bool Empty();
		bool Full();

		uint8_t m_Fullness;
		Direction m_Direction{ Direction::none };
	};

	class GridComponent final : public Component
	{
	public:
		GridComponent(GameObject* owner);
		~GridComponent() = default;
		GridComponent(const GridComponent& other) = delete;
		GridComponent(GridComponent&& other) = delete;
		GridComponent& operator=(const GridComponent& other) = delete;
		GridComponent& operator=(GridComponent&& other) = delete;

		virtual void Update() override;

		static constexpr uint8_t HEIGHT{ 14 };
		static constexpr uint8_t WIDTH{ 16 };

	private:
		struct RectI8
		{
			uint8_t left;
			uint8_t top;
			uint8_t width;
			uint8_t height;
		};
		using grid_array = std::array<std::array<GridBlock, HEIGHT>, WIDTH>;

		grid_array m_Grid;

		grid_array CreateGrid(uint8_t level);
		void SetRect(grid_array& grid, RectI8 rect, GridBlock block);
	};
}

#endif
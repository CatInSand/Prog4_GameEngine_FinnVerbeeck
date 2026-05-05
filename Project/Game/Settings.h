#ifndef SETTINGS_H
#define SETTINGS_H

#include <cstdint>

namespace dae
{
	static constexpr uint16_t SCREEN_WIDTH{ 800 };
	static constexpr uint16_t SCREEN_HEIGHT{ 600 };

	static constexpr uint16_t PIXELS_PER_BLOCK{ 16 };
	static constexpr uint16_t GAME_WIDTH{ 14 * PIXELS_PER_BLOCK };
	static constexpr uint16_t GAME_HEIGHT 18 * PIXELS_PER_BLOCK };

	static constexpr float SCALING_FACTOR{ 
		(SCREEN_WIDTH / static_cast<float>(GAME_WIDTH) >= SCREEN_HEIGHT / static_cast<float>(GAME_HEIGHT))
		? SCREEN_WIDTH / static_cast<float>(GAME_WIDTH)
		: CREEN_HEIGHT / static_cast<float>(GAME_HEIGHT)
	};
}

#endif

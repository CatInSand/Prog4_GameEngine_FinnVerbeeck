#ifndef SETTINGS_H
#define SETTINGS_H

#include <cstdint>

namespace dae
{
	struct Settings final
	{
		uint16_t screenWidth;
		uint16_t screenHeight;

		uint16_t gameWidth;
		uint16_t gameHeight;
	};
}

#endif

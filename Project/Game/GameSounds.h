#ifndef GAME_SOUNDS_H
#define GAME_SOUNDS_H

#include "ServiceLocator.h"

namespace dae
{
	enum class Sounds : sound_id {
		game_music,
		game_over,
		stage_clear,
		player_pump,
		player_shoot,
		enemy_blow,
	};

	const std::unordered_map<dae::sound_id, std::string> gIdPathMap{
		{ static_cast<sound_id>(Sounds::game_music), "audio/game_music.mp3"},
		{ static_cast<sound_id>(Sounds::game_over), "audio/game_over.mp3"},
		{ static_cast<sound_id>(Sounds::stage_clear), "audio/stage_clear.mp3"},
		{ static_cast<sound_id>(Sounds::player_pump), "audio/player_pump.mp3"},
		{ static_cast<sound_id>(Sounds::player_shoot), "audio/player_shoot.mp3"},
		{ static_cast<sound_id>(Sounds::enemy_blow), "audio/enemy_blow.mp3"},
	};
}

#endif

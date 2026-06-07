#ifndef TAGS_H
#define TAGS_H

#include "cat/hash.h"
using namespace cat::hash_literals;

namespace dae
{
	constexpr cat::hash_t TAG_DANGEROUS_TO_PLAYER{ "dangerous_to_player"_h };
	constexpr cat::hash_t TAG_DANGEROUS_TO_ENEMY{ "dangerous_to_enemy"_h };
}

#endif

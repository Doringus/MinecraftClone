#pragma once

#include <cstdint>

namespace game::world {

	enum class BlockId : uint8_t {
		Air = 0,
		Tree,
		Grass,
		Dirt,
		Stone,
		Water,
		Sand
	};

}
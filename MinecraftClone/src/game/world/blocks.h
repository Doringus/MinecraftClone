#pragma once

#include <cstdint>

namespace game::world {

	enum class BlockType : uint8_t {
		Air = 0,
		Tree,
		Grass,
		Dirt,
		Stone,
		Water,
		Sand,
		Snow,
		Leaves
	};

	static uint8_t blockTypeToRaw(const BlockType& type) {
		return static_cast<uint8_t>(type);
	}

}
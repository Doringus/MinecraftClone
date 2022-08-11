#pragma once

#include <cstdint>

#include "../../../container3d.h"
#include "../blocks.h"

namespace game::world {

	struct chunkBox_t;

	class IWorldGenerator {
	public:
		virtual ~IWorldGenerator() noexcept = default;

		virtual utils::Container3d<BlockId> createChunk(const chunkBox_t& chunk) const = 0;
	};

}
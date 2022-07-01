#pragma once

#include <cstdint>

#include "../../container3d.h"

namespace game {
	namespace world {

		struct chunkBox_t;

		class IWorldGenerator {
		public:
			virtual ~IWorldGenerator() noexcept = default;

			virtual utils::Container3d<uint16_t> createChunk(const chunkBox_t& chunk) const = 0;
		};

	}
}
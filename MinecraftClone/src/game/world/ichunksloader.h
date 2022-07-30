#pragma once

#include <memory>

#include "chunk.h"


namespace game::world {

	class IChunksLoader {
	public:
		virtual ~IChunksLoader() = default;

		virtual utils::Container3d<uint16_t> loadChunkBlocks(const chunkBox_t& box) = 0;
		virtual void storeChunk(std::unique_ptr<chunk_t>) = 0;
	};

}
#include "chunksfactory.h"

#include "chunk.h"

namespace game::world {

	ChunksFactory::ChunksFactory(graphics::ChunkRenderer* renderer, const BlocksDatabase& blocksDatabase) : m_Renderer(renderer), m_BlocksDatabase(blocksDatabase) { }

	std::unique_ptr<chunk_t> ChunksFactory::createChunk(const chunkBox_t& box) {
		return std::unique_ptr<chunk_t>();
	}

}
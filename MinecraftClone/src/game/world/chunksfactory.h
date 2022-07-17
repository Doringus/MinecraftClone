#pragma once

#include "blocksdatabase.h"

#include <memory>

namespace graphics {
	class ChunkRenderer;
}

namespace game::world {

	struct chunk_t;
	struct chunkBox_t;

	class ChunksFactory {
	public:
		ChunksFactory(graphics::ChunkRenderer* renderer, const BlocksDatabase& blocksDatabase);

		std::unique_ptr<chunk_t> createChunk(const chunkBox_t& box);
	private:
		graphics::ChunkRenderer* m_Renderer;
		BlocksDatabase m_BlocksDatabase;
	};

}
#include "chunkshashmapstorage.h"

namespace game::world {

	std::optional<chunk_t*> ChunksHashmapStorage::getChunk(int64_t x, int64_t z) const {
		if (m_Chunks.contains({ x, z })) {
			return m_Chunks.at({x, z}).get();
		}
		return std::nullopt;
	}

	std::optional<std::unique_ptr<chunk_t>> ChunksHashmapStorage::takeChunk(int64_t x, int64_t z) {
		if (m_Chunks.contains({ x, z })) {
			auto chunkPtr = std::move(m_Chunks.at({ x, z }));
			removeChunk(x, z);
			return chunkPtr;
		}
		return std::nullopt;
	}

	void ChunksHashmapStorage::insertChunk(std::unique_ptr<chunk_t> chunk) {
		m_Chunks[{chunk->box.xGrid, chunk->box.zGrid}] = std::move(chunk);
	}

	void ChunksHashmapStorage::removeChunk(int64_t x, int64_t z) {
		m_Chunks.erase({ x, z });
	}

}
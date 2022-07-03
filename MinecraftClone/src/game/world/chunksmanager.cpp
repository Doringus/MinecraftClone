#include "chunksmanager.h"

namespace game::world {

	ChunksManager::ChunksManager(BlocksDatabase blocksDatabase, std::unique_ptr<IWorldGenerator> worldGenerator) noexcept : m_BlocksDatabase(std::move(blocksDatabase)), 
		m_WorldGenerator(std::move(worldGenerator)) {
	}

	void ChunksManager::setWorldSize(size_t size) noexcept {
		m_Size = size;
	}

	void ChunksManager::updateTerrain(int64_t chunkX, int64_t chunkZ) {
	}
}
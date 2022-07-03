#pragma once

#include <memory>

#include "blocksdatabase.h"
#include "chunksstorage.h"

namespace game::world {

	class IWorldGenerator;

	class ChunksManager {
	public:
		ChunksManager(BlocksDatabase blocksDatabase, std::unique_ptr<IWorldGenerator> worldGenerator) noexcept;

		void setWorldSize(size_t size) noexcept;
		void updateTerrain(int64_t chunkX, int64_t chunkZ);
	private:
		BlocksDatabase m_BlocksDatabase;
		std::unique_ptr<IWorldGenerator> m_WorldGenerator;
		size_t m_Size = 3;
		int64_t m_CurrentChunkX = 0, m_CurrentChunkZ = 0;
		ChunksStorage m_Storage;
	};
}
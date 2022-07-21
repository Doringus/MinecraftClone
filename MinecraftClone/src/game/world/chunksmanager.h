#pragma once

#include "ichunksstorage.h"
#include "blocksdatabase.h"

namespace graphics {
	class ChunkRenderer;
}

namespace utils {
	class ThreadPool;
}

namespace game::world {

	class IWorldGenerator;

	class ChunksManager {

		struct worldBox_t {
			int64_t bottomLeftX, bottomLeftXWithShadowChunks;
			int64_t bottomLeftZ, bottomLeftZWithShadowChunks;
			size_t size, sizeWithShadowChunks;
		};

	public:
		ChunksManager(size_t worldSize, const BlocksDatabase& blocksDatabase, 
			std::unique_ptr<IWorldGenerator> worldGenerator, graphics::ChunkRenderer* renderer, utils::ThreadPool* threadPool) noexcept;

		void updateChunks(int64_t x, int64_t z);
		void submitChunksToRenderer();
	private:
		std::unique_ptr<chunk_t> createEmptyChunk(const chunkBox_t& box);
		std::unique_ptr<chunk_t> loadChunkFromDisk(); // todo later
		std::vector<std::unique_ptr<chunk_t>> getChunksToRewrite(int64_t offsetX, int64_t offsetZ);
		void rewriteChunks(std::vector<std::unique_ptr<chunk_t>>& chunks) const;
		void updateChunkData(const std::unique_ptr<chunk_t>& chunk) const;
	private:
		BlocksDatabase m_BlocksDatabase;
		worldBox_t m_WorldBox;
		int64_t m_CurrentChunkX = 0, m_CurrentChunkZ = 0;
		size_t m_WorldSize;
		graphics::ChunkRenderer* m_Renderer;
		utils::ThreadPool* m_ThreadPool;
		std::unique_ptr<IChunksStorage> m_ChunksStorage;
		std::unique_ptr<IWorldGenerator> m_WorldGenerator;
	};

}
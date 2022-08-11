#pragma once

#include <unordered_set>

#include "ichunksloader.h"
#include "blocksdatabase.h"
#include "chunkshashmapstorage.h"

namespace graphics {
	class ChunkRenderer;
}

namespace utils {
	class ThreadPool;
}

namespace game::world {

	class IWorldGenerator;

	class SingleplayerChunksLoader : public IChunksLoader {
	public:
		SingleplayerChunksLoader(std::unique_ptr<IWorldGenerator> worldGenerator);

		utils::Container3d<uint16_t> loadChunkBlocks(const chunkBox_t& box) override;
		void storeChunk(std::unique_ptr<chunk_t> chunk) override;
	private:
		utils::Container3d<uint16_t> loadChunkFromDisk(const chunkBox_t& box);
	private:
		std::unique_ptr<IWorldGenerator> m_WorldGenerator;
		std::unordered_set<std::pair<int64_t, int64_t>, chunkHash_t> m_StoredChunks;
	};

}
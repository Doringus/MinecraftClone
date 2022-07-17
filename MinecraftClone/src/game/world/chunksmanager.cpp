#include "chunksmanager.h"

#include "chunkshashmapstorage.h"
#include "../../renderer/chunkrenderer.h"
#include "dummyworldgenerator.h"
#include "../../../vendor/glm/ext/matrix_transform.hpp"

namespace game::world {

	int64_t getChunkCoord(int64_t coord, size_t chunkSize) {
		if (coord >= 0) {
			return ((coord / chunkSize) * chunkSize);
		}
		else {
			return ((coord / chunkSize) * chunkSize - chunkSize);
		}
	}

	ChunksManager::ChunksManager(size_t worldSize, const BlocksDatabase& blocksDatabase,
								 std::unique_ptr<IWorldGenerator> worldGenerator, graphics::ChunkRenderer* renderer) noexcept :
								 m_WorldSize(worldSize), m_BlocksDatabase(blocksDatabase), m_WorldGenerator(std::move(worldGenerator)),
								 m_Renderer(renderer) {

		m_WorldBox = { (int64_t)0 - (int64_t)m_WorldSize, (int64_t)0 - (int64_t)worldSize, worldSize * 2 + 1 };
		m_ChunksStorage = std::make_unique<ChunksHashmapStorage>();
		for (int64_t i = m_WorldBox.bottomLeftX; i < int64_t(m_WorldBox.size) + m_WorldBox.bottomLeftX; ++i) {
			for (int64_t j = m_WorldBox.bottomLeftZ; j <int64_t(m_WorldBox.size) + m_WorldBox.bottomLeftZ; ++j) {
				auto chunk = createEmptyChunk({ i, j, 16, 256, 16 });
				chunk->blocks = m_WorldGenerator->createChunk(chunk->box);
				createChunkMesh(m_BlocksDatabase, *chunk);
				m_ChunksStorage->insertChunk(std::move(chunk));
			}
		}
	}

	void ChunksManager::updateChunks(int64_t x, int64_t z) {
		int64_t chunkX = getChunkCoord(x, 16);
		int64_t chunkZ = getChunkCoord(z, 16);
		int64_t offsetX = chunkX - m_CurrentChunkX;
		int64_t offsetZ = chunkZ - m_CurrentChunkZ;
		if (offsetX != 0 || offsetZ != 0) {
		//	auto chunkToRewrite = getChunksToRewrite();
		}

	}

	void ChunksManager::submitChunksToRenderer() {
		for (int64_t i = m_WorldBox.bottomLeftX; i < int64_t(m_WorldBox.size) + m_WorldBox.bottomLeftX; ++i) {
			for (int64_t j = m_WorldBox.bottomLeftZ; j < int64_t(m_WorldBox.size) + m_WorldBox.bottomLeftZ; ++j) {
				m_Renderer->submit(m_ChunksStorage->getChunk(i, j).value()->renderData);
			}
		}
	}

	std::unique_ptr<chunk_t> ChunksManager::createEmptyChunk(const chunkBox_t& box) {
		auto renderItem = m_Renderer->createChunkRenderData();
		renderItem->setModelMatrix(glm::translate(glm::mat4(1.0), glm::vec3(box.xGrid * int64_t(box.width), 0, box.zGrid * int64_t(box.depth))));
		return std::make_unique<chunk_t>(box, utils::Container3d<uint16_t>(box.width, box.height, box.depth), renderItem);
	}

	std::unique_ptr<chunk_t> ChunksManager::loadChunkFromDisk() {
		return std::unique_ptr<chunk_t>();
	}

	std::vector<chunk_t*> ChunksManager::getChunksToRewrite() {
		return std::vector<chunk_t*>();
	}

}
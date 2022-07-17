#include "chunksmanager.h"

#include "chunkshashmapstorage.h"
#include "../../renderer/chunkrenderer.h"
#include "dummyworldgenerator.h"
#include "../../../vendor/glm/ext/matrix_transform.hpp"

namespace game::world {

	int64_t getChunkCoord(int64_t coord, int64_t chunkSize) {
		if (coord >= 0) {
			return (coord / chunkSize);
		} else {
			return ((coord / chunkSize) - 1);
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
				updateChunkData(chunk);
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
			std::vector<std::unique_ptr<chunk_t>> chunksToRewrite = getChunksToRewrite(offsetX, offsetZ);
			m_CurrentChunkX = chunkX;
			m_CurrentChunkZ = chunkZ;
			m_WorldBox.bottomLeftX += offsetX;
			m_WorldBox.bottomLeftZ += offsetZ;
			rewriteChunks(chunksToRewrite);
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
		return std::make_unique<chunk_t>(box, utils::Container3d<uint16_t>(box.width, box.height, box.depth), renderItem);
	}

	std::unique_ptr<chunk_t> ChunksManager::loadChunkFromDisk() {
		return std::unique_ptr<chunk_t>();
	}

	std::vector<std::unique_ptr<chunk_t>> ChunksManager::getChunksToRewrite(int64_t offsetX, int64_t offsetZ) {
		std::vector<std::unique_ptr<chunk_t>> result;
		if (offsetX > 0) {
			for (int64_t i = m_WorldBox.bottomLeftX; i < m_WorldBox.bottomLeftX + offsetX; ++i) {
				for (int64_t j = m_WorldBox.bottomLeftZ; j < m_WorldBox.bottomLeftZ + (int64_t)m_WorldBox.size; ++j) {
					if (auto chunk = m_ChunksStorage->takeChunk(i, j); chunk) {
						result.push_back(std::move(*chunk));
					}
				}
			}
		} else {
			for (int64_t i = m_WorldBox.bottomLeftX + (int64_t)m_WorldBox.size + offsetX; i < m_WorldBox.bottomLeftX + (int64_t)m_WorldBox.size; ++i) {
				for (int64_t j = m_WorldBox.bottomLeftZ; j < m_WorldBox.bottomLeftZ + (int64_t)m_WorldBox.size; ++j) {
					if (auto chunk = m_ChunksStorage->takeChunk(i, j); chunk) {
						result.push_back(std::move(*chunk));
					}
				}
			}
		}

		if (offsetZ > 0) {
			for (int64_t j = m_WorldBox.bottomLeftZ; j < m_WorldBox.bottomLeftZ + offsetZ; ++j) {
				for (int64_t i = m_WorldBox.bottomLeftX; i < m_WorldBox.bottomLeftX + (int64_t)m_WorldBox.size; ++i) {
					if (auto chunk = m_ChunksStorage->takeChunk(i, j); chunk) {
						result.push_back(std::move(*chunk));
					}
				}
			}
		} else {
			for (int64_t j = m_WorldBox.bottomLeftZ + (int64_t)m_WorldBox.size + offsetZ; j < m_WorldBox.bottomLeftZ + (int64_t)m_WorldBox.size; ++j) {
				for (int64_t i = m_WorldBox.bottomLeftX; i < m_WorldBox.bottomLeftX + (int64_t)m_WorldBox.size; ++i) {
					if (auto chunk = m_ChunksStorage->takeChunk(i, j); chunk) {
						result.push_back(std::move(*chunk));
					}
				}
			}
		}

		return result;
	}

	void ChunksManager::rewriteChunks(std::vector<std::unique_ptr<chunk_t>>& chunks) const {
		for (int64_t i = m_WorldBox.bottomLeftX; i < int64_t(m_WorldBox.size) + m_WorldBox.bottomLeftX; ++i) {
			for (int64_t j = m_WorldBox.bottomLeftZ; j < int64_t(m_WorldBox.size) + m_WorldBox.bottomLeftZ; ++j) {
				if (!m_ChunksStorage->contains(i, j)) {
					auto chunk = std::move(chunks.back());
					chunks.pop_back();
					chunk->box.xGrid = i;
					chunk->box.zGrid = j;
					updateChunkData(chunk);
					m_ChunksStorage->insertChunk(std::move(chunk));
				}
			}
		}
	}

	void ChunksManager::updateChunkData(const std::unique_ptr<chunk_t>& chunk) const {
		chunk->renderData->setModelMatrix(glm::translate(glm::mat4(1.0), glm::vec3(chunk->box.xGrid * int64_t(chunk->box.width), 
			0, chunk->box.zGrid * int64_t(chunk->box.depth))));
		chunk->blocks = m_WorldGenerator->createChunk(chunk->box);
		createChunkMesh(m_BlocksDatabase, *chunk);
	}

}
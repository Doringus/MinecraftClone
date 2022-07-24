#include "chunksmanager.h"

#include "chunkshashmapstorage.h"
#include "../../renderer/chunkrenderer.h"
#include "dummyworldgenerator.h"
#include "../../../vendor/glm/ext/matrix_transform.hpp"

#include <chrono>
#include <spdlog/spdlog.h>

#include "../../threading/threadpool.h"

namespace game::world {

	int64_t getChunkCoord(int64_t coord, int64_t chunkSize) {
		if (coord >= 0) {
			return (coord / chunkSize);
		} else {
			return ((coord / chunkSize) - 1);
		}
	}

	ChunksManager::ChunksManager(size_t worldSize, const BlocksDatabase& blocksDatabase, std::unique_ptr<IWorldGenerator> worldGenerator, 
							     graphics::ChunkRenderer* renderer, utils::ThreadPool* threadPool) noexcept :
								 m_WorldSize(worldSize), m_BlocksDatabase(blocksDatabase), m_WorldGenerator(std::move(worldGenerator)),
								 m_Renderer(renderer), m_ThreadPool(threadPool) {

		m_WorldBox = { (int64_t)0 - (int64_t)m_WorldSize, (int64_t)0 - (int64_t)worldSize, worldSize * 2 + 1};
		m_ChunksStorage = std::make_unique<ChunksHashmapStorage>();
		auto t1 = std::chrono::steady_clock::now();
		std::vector<std::future<void>> chunkTasks;
		chunkTasks.reserve((m_WorldBox.size + 1) << 1);
		std::vector<std::unique_ptr<chunk_t>> worldChunks;
		for (int64_t i = m_WorldBox.bottomLeftX - 1; i < int64_t(m_WorldBox.size) + m_WorldBox.bottomLeftX + 1; ++i) {
			for (int64_t j = m_WorldBox.bottomLeftZ - 1; j <int64_t(m_WorldBox.size) + m_WorldBox.bottomLeftZ + 1; ++j) {
				auto chunk = createEmptyChunk({ i, j, 16, 256, 16 });
				auto task = m_ThreadPool->submit([this, i, j, chunkPtr = chunk.get()]() {
					
					chunkPtr->blocks = m_WorldGenerator->createChunk(chunkPtr->box);
					chunkPtr->renderData->setModelMatrix(glm::translate(glm::mat4(1.0), glm::vec3(chunkPtr->box.xGrid * int64_t(chunkPtr->box.width),
						0, chunkPtr->box.zGrid * int64_t(chunkPtr->box.depth))));
					chunkPtr->renderData->getIndexBuffer()->clear();
					chunkPtr->renderData->getVertexBuffer()->clear();
				});
				
				chunkTasks.push_back(std::move(task));
				worldChunks.push_back(std::move(chunk));
			}
		}
		utils::waitForAllTasks(chunkTasks);
		chunkTasks.clear();
		for (auto& chunk : worldChunks) {
			auto task = m_ThreadPool->submit([this, chunkPtr = chunk.get()]() {
				createChunkMesh(*m_ChunksStorage, m_BlocksDatabase, *chunkPtr);
			});
			chunkTasks.push_back(std::move(task));
			m_ChunksStorage->insertChunk(std::move(chunk));
		}
		utils::waitForAllTasks(chunkTasks);

		auto t2 = std::chrono::steady_clock::now();
		auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
		spdlog::info("World created {0}", elapsed_ms.count());
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
			for (int64_t i = m_WorldBox.bottomLeftX + (int64_t)m_WorldBox.size + offsetX;
				i < m_WorldBox.bottomLeftX + (int64_t)m_WorldBox.size; ++i) {
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
			for (int64_t j = m_WorldBox.bottomLeftZ + (int64_t)m_WorldBox.size + offsetZ;
				j < m_WorldBox.bottomLeftZ + (int64_t)m_WorldBox.size; ++j) {
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
		chunk->renderData->getIndexBuffer()->clear();
		chunk->renderData->getVertexBuffer()->clear();
		chunk->blocks = m_WorldGenerator->createChunk(chunk->box);
		createChunkMesh(*m_ChunksStorage, m_BlocksDatabase, *chunk);
	}

}
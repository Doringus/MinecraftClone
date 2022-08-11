#include "chunksmanager.h"

#include "chunkshashmapstorage.h"
#include "../../renderer/chunkrenderer.h"
#include "generators/iworldgenerator.h"
#include "../../../vendor/glm/ext/matrix_transform.hpp"
#include "ichunksloader.h"

#include <chrono>
#include <fstream>
#include <spdlog/spdlog.h>
#include <sstream>

#include "../../threading/threadpool.h"

namespace game::world {

	int64_t getChunkCoord(int64_t coord, int64_t chunkSize) {
		if (coord >= 0) {
			return (coord / chunkSize);
		} else {
			return ((coord / chunkSize) - 1);
		}
	}

	ChunksManager::ChunksManager(const WorldBox& worldBox, const BlocksDatabase& blocksDatabase, std::unique_ptr<IChunksLoader> chunksLoader,
		graphics::ChunkRenderer* renderer, utils::ThreadPool* threadPool) noexcept :
								 m_WorldBox(worldBox), m_BlocksDatabase(blocksDatabase), m_ChunksLoader(std::move(chunksLoader)),
								 m_Renderer(renderer), m_ThreadPool(threadPool) {

		auto t1 = std::chrono::steady_clock::now();
		std::vector<std::future<void>> chunkTasks;
		chunkTasks.reserve((m_WorldBox.size() + m_WorldBox.shadowArea()) << 1);
		std::vector<std::unique_ptr<chunk_t>> worldChunks;
		for (int64_t i = m_WorldBox.bottomLeftShadow().x; i <= m_WorldBox.bottomRightShadow().x; ++i) {
			for (int64_t j = m_WorldBox.bottomLeftShadow().y; j <= m_WorldBox.topRightShadow().y; ++j) {
				auto chunk = createEmptyChunk({ i, j, 16, 256, 16 });
				auto task = m_ThreadPool->submit([this, i, j, chunkPtr = chunk.get()]() {
					chunkPtr->blocks = m_ChunksLoader->loadChunkBlocks(chunkPtr->box);
					chunkPtr->renderData->setModelMatrix(glm::translate(glm::mat4(1.0), glm::vec3(chunkPtr->box.xGrid * int64_t(chunkPtr->box.width),
						0, chunkPtr->box.zGrid * int64_t(chunkPtr->box.depth))));
				});
				
				chunkTasks.push_back(std::move(task));
				worldChunks.push_back(std::move(chunk));
			}
		}
		utils::waitForAllTasks(chunkTasks);
		chunkTasks.clear();
		for (auto& chunk : worldChunks) {
			auto task = m_ThreadPool->submit([this, chunkPtr = chunk.get()]() {
				createChunkMesh(m_ChunksStorage, m_BlocksDatabase, *chunkPtr);
			});
			chunkTasks.push_back(std::move(task));
			m_ChunksStorage.insertChunk(std::move(chunk));
		}
		utils::waitForAllTasks(chunkTasks);

		auto t2 = std::chrono::steady_clock::now();
		auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
		spdlog::info("World created {0}", elapsed_ms.count());
	}

	BlockId ChunksManager::getBlockAt(int64_t x, int64_t y, int64_t z) {
		int64_t chunkX = getChunkCoord(x, 16);
		int64_t chunkZ = getChunkCoord(z, 16);
		auto chunk = *m_ChunksStorage.getChunk(x, z);
		return chunk->blocks.get(std::abs(x) % 16, std::abs(y) % 16, std::abs(z) % 16);
	}

	void ChunksManager::update(int64_t x, int64_t z, double dt) {
		updateExpiringChunks(dt);
		int64_t chunkX = getChunkCoord(x, 16);
		int64_t chunkZ = getChunkCoord(z, 16);
		int64_t offsetX = chunkX - m_CurrentChunkX;
		int64_t offsetZ = chunkZ - m_CurrentChunkZ;
		if (offsetX != 0 || offsetZ != 0) {
			addExpiringChunks(offsetX, offsetZ);
			m_CurrentChunkX = chunkX;
			m_CurrentChunkZ = chunkZ;
			m_WorldBox.setBottomLeftCorner({ m_WorldBox.bottomLeft().x + offsetX, m_WorldBox.bottomLeft().y + offsetZ });
			updateWorldChunks();
		}
	}

	void ChunksManager::submitChunksToRenderer() {
		for (int64_t i = m_WorldBox.bottomLeft().x; i <= m_WorldBox.bottomRight().x; ++i) {
			for (int64_t j = m_WorldBox.bottomLeft().y; j <= m_WorldBox.topRight().y; ++j) {
				m_Renderer->submit(m_ChunksStorage.getChunk(i, j).value()->renderData);
			}
		}
	}

	std::unique_ptr<chunk_t> ChunksManager::createEmptyChunk(const chunkBox_t& box) {
		auto renderItem = m_Renderer->createChunkRenderData();
		return std::make_unique<chunk_t>(box, utils::Container3d<BlockId>(box.width, box.height, box.depth), renderItem);
	}

	std::unique_ptr<chunk_t> ChunksManager::loadChunkFromDisk() {
		return std::unique_ptr<chunk_t>();
	}

	void ChunksManager::addExpiringChunks(int64_t offsetX, int64_t offsetZ) {
		if (offsetX > 0) {
			for (int64_t i = m_WorldBox.bottomLeftShadow().x; i < m_WorldBox.bottomLeftShadow().x + offsetX; ++i) {
				for (int64_t j = m_WorldBox.bottomLeftShadow().y; j <= m_WorldBox.topLeftShadow().y; ++j) {
					if (auto chunk = m_ChunksStorage.takeChunk(i, j); chunk) {
						m_ExpiringChunks.insertChunk(std::move(*chunk));
					}
				}
			}
		} else {
			for (int64_t i = m_WorldBox.bottomRightShadow().x + offsetX; i < m_WorldBox.bottomRightShadow().x; ++i) {
				for (int64_t j = m_WorldBox.bottomRightShadow().y; j <= m_WorldBox.topRightShadow().y; ++j) {
					if (auto chunk = m_ChunksStorage.takeChunk(i, j); chunk) {
						m_ExpiringChunks.insertChunk(std::move(*chunk));
					}
				}
			}
		}

		if (offsetZ > 0) {
			for (int64_t j = m_WorldBox.bottomLeftShadow().y; j < m_WorldBox.bottomLeftShadow().y + offsetZ; ++j) {
				for (int64_t i = m_WorldBox.bottomLeftShadow().x; i <= m_WorldBox.bottomRightShadow().x; ++i) {
					if (auto chunk = m_ChunksStorage.takeChunk(i, j); chunk) {
						m_ExpiringChunks.insertChunk(std::move(*chunk));
					}
				}
			}
		} else {
			for (int64_t j = m_WorldBox.topLeftShadow().y + offsetZ; j < m_WorldBox.topLeftShadow().y; ++j) {
				for (int64_t i = m_WorldBox.bottomLeftShadow().x; i <= m_WorldBox.bottomRightShadow().x; ++i) {
					if (auto chunk = m_ChunksStorage.takeChunk(i, j); chunk) {
						m_ExpiringChunks.insertChunk(std::move(*chunk));
					}
				}
			}
		}
	}

	void ChunksManager::updateExpiringChunks(double dt) {
		for (auto& [coor, chunk] : m_ExpiringChunks) {
			chunk->m_TimeToLive -= dt;
		}

		for (auto it = m_ExpiringChunks.begin(); it != m_ExpiringChunks.end();) {
			if (it->second->m_TimeToLive < 0) {
				spdlog::info("Erasing chunk x: {0}, y: {1}", it->second->box.xGrid, it->second->box.zGrid);
				m_ChunksLoader->storeChunk(std::move((*it).second));
				it = m_ExpiringChunks.erase(it);
			} else {
				it++;
			}
		}
	}

	void ChunksManager::updateWorldChunks() {
		std::vector<std::future<void>> chunkTasks;
		std::vector<chunk_t*> worldChunks;
		for (int64_t i = m_WorldBox.bottomLeftShadow().x; i < m_WorldBox.bottomRightShadow().x; ++i) {
			for (int64_t j = m_WorldBox.bottomLeftShadow().y; j < m_WorldBox.topRightShadow().y; ++j) {
				if (!m_ChunksStorage.contains(i, j) && !m_ExpiringChunks.contains(i, j)) {
					auto chunk = createEmptyChunk({ i, j, 16, 256, 16 });
					auto task = m_ThreadPool->submit([this, i, j, chunkPtr = chunk.get()]() {

						chunkPtr->blocks = m_ChunksLoader->loadChunkBlocks(chunkPtr->box);
						chunkPtr->renderData->setModelMatrix(glm::translate(glm::mat4(1.0), glm::vec3(chunkPtr->box.xGrid* int64_t(chunkPtr->box.width),
							0, chunkPtr->box.zGrid* int64_t(chunkPtr->box.depth))));
					});

					chunkTasks.push_back(std::move(task));
					worldChunks.push_back(chunk.get());
					m_ChunksStorage.insertChunk(std::move(chunk));
				} else {
					std::optional<chunk_t*> chunk;
					if (chunk = m_ChunksStorage.getChunk(i, j); !chunk) {
						auto chunkPtr = m_ExpiringChunks.takeChunk(i, j);
						chunk = (*chunkPtr).get();
						m_ChunksStorage.insertChunk(std::move(*chunkPtr));
					}
					if ((*chunk)->renderData->getVertexBuffer()->elementsCount() == 0) {
						worldChunks.push_back(*chunk);
					}
				}
			}
		}
		utils::waitForAllTasks(chunkTasks);
		chunkTasks.clear();
		for (int64_t i = m_WorldBox.bottomLeft().x; i < m_WorldBox.bottomRight().x; ++i) {
			for (int64_t j = m_WorldBox.bottomLeft().y; j < m_WorldBox.topRight().y; ++j) {
				auto chunk = m_ChunksStorage.getChunk(i, j);
				auto task = m_ThreadPool->submit([this, chunk]() {
					createChunkMesh(m_ChunksStorage, m_BlocksDatabase, **chunk);
					});
				chunkTasks.push_back(std::move(task));
			}
		}
	/*	for (auto& chunk : worldChunks) {
			auto task = m_ThreadPool->submit([this, chunk]() {
				createChunkMesh(m_ChunksStorage, m_BlocksDatabase, *chunk);
			});
			chunkTasks.push_back(std::move(task));
		}*/
	//	utils::waitForAllTasks(chunkTasks);
	}

}
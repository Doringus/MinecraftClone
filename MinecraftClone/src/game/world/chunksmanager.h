#pragma once

#include "blocksdatabase.h"
#include "chunkshashmapstorage.h"

#include "../../../vendor/glm/vec2.hpp"

namespace graphics {
	class ChunkRenderer;
}

namespace utils {
	class ThreadPool;
}

namespace glm {

	using lvec2 = vec<2, int64_t, defaultp>;

}

namespace game::world {

	class IWorldGenerator;
	class IChunksLoader;

	class WorldBox {
	public:

		WorldBox(glm::lvec2 bottomLeft, size_t size, size_t shadowArea) noexcept : m_Size(size), m_ShadowArea(shadowArea) {
			setBottomLeftCorner(bottomLeft);
		}

		void setBottomLeftCorner(glm::lvec2 bottomLeft) {
			m_BottomLeft = bottomLeft;
			m_BottomRight = { bottomLeft.x + m_Size, bottomLeft.y };
			m_TopLeft = { bottomLeft.x, bottomLeft.y + m_Size }; 
			m_TopRight = { bottomLeft.x + m_Size, bottomLeft.y + m_Size };
			m_BottomLeftShadow = { bottomLeft.x - m_ShadowArea, bottomLeft.y - m_ShadowArea };
			m_BottomRightShadow = { m_BottomRight.x + m_ShadowArea, m_BottomRight.y - m_ShadowArea };
			m_TopLeftShadow = { m_TopLeft.x - m_ShadowArea, m_TopLeft.y + m_ShadowArea };
			m_TopRightShadow = { m_TopRight.x + m_ShadowArea, m_TopRight.y + m_ShadowArea };
		}

		glm::lvec2 bottomLeft() const noexcept {
			return m_BottomLeft;
		}

		glm::lvec2 bottomRight() const noexcept {
			return m_BottomRight;
		}

		glm::lvec2 topLeft() const noexcept {
			return m_TopLeft;
		}

		glm::lvec2 topRight() const noexcept {
			return m_TopRight;
		}

		glm::lvec2 bottomLeftShadow() const noexcept {
			return m_BottomLeftShadow;
		}

		glm::lvec2 bottomRightShadow() const noexcept {
			return m_BottomRightShadow;
		}

		glm::lvec2 topLeftShadow() const noexcept {
			return m_TopLeftShadow;
		}

		glm::lvec2 topRightShadow() const noexcept {
			return m_TopRightShadow;
		}

		size_t size() const noexcept {
			return m_Size;
		}

		size_t shadowArea() const noexcept {
			return m_ShadowArea;
		}

	private:
		size_t m_Size, m_ShadowArea;
		glm::lvec2 m_BottomLeft, m_BottomRight, m_BottomLeftShadow, m_BottomRightShadow;
		glm::lvec2 m_TopLeft, m_TopRight, m_TopLeftShadow, m_TopRightShadow;
	};

	class ChunksManager {
	public:
		ChunksManager(const WorldBox& worldBox, const BlocksDatabase& blocksDatabase, std::unique_ptr<IChunksLoader> chunksLoader,
			graphics::ChunkRenderer* renderer, utils::ThreadPool* threadPool) noexcept;

		void update(int64_t x, int64_t z, double dt);
		void submitChunksToRenderer();
	private:
		std::unique_ptr<chunk_t> createEmptyChunk(const chunkBox_t& box);
		std::unique_ptr<chunk_t> loadChunkFromDisk(); // todo later
		void addExpiringChunks(int64_t offsetX, int64_t offsetZ);
		void updateExpiringChunks(double dt);
		void updateWorldChunks();
	private:
		BlocksDatabase m_BlocksDatabase;
		ChunksHashmapStorage m_ChunksStorage, m_ExpiringChunks;
		WorldBox m_WorldBox;
		int64_t m_CurrentChunkX = 0, m_CurrentChunkZ = 0;
		graphics::ChunkRenderer* m_Renderer;
		utils::ThreadPool* m_ThreadPool;
		std::unique_ptr<IChunksLoader> m_ChunksLoader;
	};

}
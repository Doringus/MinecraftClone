#include "chunkrenderer.h"

#include "camera.h"

namespace graphics {
	
	bool ChunkRenderer::ChunkRenderData::updated() const noexcept {
		return m_IsUpdated;
	}
	void ChunkRenderer::ChunkRenderData::beginChunk() noexcept {
		m_Vertices.clear();
		m_Indices.clear();
		m_IsUpdated = true;
	}
	void ChunkRenderer::ChunkRenderData::markUpdated() noexcept {
		m_IsUpdated = true;
	}
	void ChunkRenderer::ChunkRenderData::markFlushedToGPU() noexcept {
		m_IsUpdated = false;
	}

	std::vector<chunkVertex_t>& ChunkRenderer::ChunkRenderData::getVertices() noexcept 	{
		return m_Vertices;
	}

	std::vector<uint32_t>& ChunkRenderer::ChunkRenderData::getIndices() noexcept {
		return m_Indices;
	}

}
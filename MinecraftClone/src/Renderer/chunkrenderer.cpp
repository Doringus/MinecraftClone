#include "chunkrenderer.h"

#include <iterator>

#include "camera.h"

namespace graphics {
	
	bool ChunkRenderer::ChunkRenderData::updated() const noexcept {
		return m_IsUpdated;
	}
	void ChunkRenderer::ChunkRenderData::beginChunk() noexcept {
		//m_Vertices.clear();
		//m_Indices.clear();
		m_IsUpdated = true;
	}
	void ChunkRenderer::ChunkRenderData::markFlushedToGPU() noexcept {
		m_IsUpdated = false;
	}

	void ChunkRenderer::ChunkRenderData::addVertices(const std::vector<chunkVertex_t>& vertices, const std::vector<uint16_t>& indices) {
		std::copy(vertices.begin(), vertices.end(), std::back_inserter(m_Vertices));
		std::copy(indices.begin(), indices.end(), std::back_inserter(m_Indices));
	}

	std::vector<chunkVertex_t>& ChunkRenderer::ChunkRenderData::getVertices() noexcept 	{
		return m_Vertices;
	}

	std::vector<uint32_t>& ChunkRenderer::ChunkRenderData::getIndices() noexcept {
		return m_Indices;
	}

}
#include "chunkrenderer.h"

#include "camera.h"

namespace graphics {

	ChunkRenderer::ChunkRenderData::ChunkRenderData(std::unique_ptr<IGpuBuffer> vertexBuffer, std::unique_ptr<IGpuBuffer> indexBuffer) :
	m_VertexBuffer(std::move(vertexBuffer)), m_IndexBuffer(std::move(indexBuffer)) { }

	IGpuBuffer* ChunkRenderer::ChunkRenderData::getVertexBuffer() noexcept {
		return m_VertexBuffer.get();
	}

	IGpuBuffer* ChunkRenderer::ChunkRenderData::getIndexBuffer() noexcept {
		return m_IndexBuffer.get();
	}

	void ChunkRenderer::ChunkRenderData::setModelMatrix(const glm::mat4& matrix) noexcept {
		m_ModelMatrix = matrix;
	}

	glm::mat4 ChunkRenderer::ChunkRenderData::getModelMatrix() const noexcept {
		return m_ModelMatrix;
	}

}
#include "renderitem.h"

namespace graphics {

	RenderItem::RenderItem(std::unique_ptr<IGpuBuffer> vertexBuffer, std::unique_ptr<IGpuBuffer> indexBuffer) noexcept 
		: m_VertexBuffer(std::move(vertexBuffer)), m_IndexBuffer(std::move(indexBuffer)) { }

	IGpuBuffer* RenderItem::getVertexBuffer() noexcept {
		return m_VertexBuffer.get();
	}

	IGpuBuffer* RenderItem::getIndexBuffer() noexcept {
		return m_IndexBuffer.get();
	}

	void RenderItem::setModelMatrix(const glm::mat4& matrix) noexcept {
		m_ModelMatrix = matrix;
	}

	glm::mat4 RenderItem::getModelMatrix() const noexcept {
		return m_ModelMatrix;
	}

}
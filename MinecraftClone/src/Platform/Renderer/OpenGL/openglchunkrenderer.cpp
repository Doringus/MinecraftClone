#include "openglchunkrenderer.h"

#include "../../../renderer/camera.h"
#include "openglgpubuffermanager.h"

namespace graphics::opengl {
		
	OpenglChunkRenderer::OpenglChunkRenderer() noexcept {
		m_ChunksToRender.reserve(100);
		m_GpuBufferManager = std::make_unique<OpenglGpuBufferManager>();
	}

	std::shared_ptr<RenderItem> OpenglChunkRenderer::createChunkRenderData() {
		return std::make_shared<RenderItem>(m_GpuBufferManager->createVertexBuffer(sizeof(chunkVertex_t)), m_GpuBufferManager->createIndexBuffer());
	}

	void OpenglChunkRenderer::submit(const std::shared_ptr<RenderItem>& chunk) {
		m_ChunksToRender.push_back(chunk);
	}

	void OpenglChunkRenderer::render(const Camera& camera) {
		glDepthFunc(GL_LESS);
		m_ChunkShader.use();
		m_ChunkTextureAtlas.bind(0);
		m_ChunkVao.bind();
		for (auto& renderable : m_ChunksToRender) {
			m_ChunkShader.setUniformMat4("mvp", camera.calculateCameraMatrix() * renderable->getModelMatrix());
			renderable->getVertexBuffer()->copyFromShadowBuffer();
			renderable->getIndexBuffer()->copyFromShadowBuffer();
			static_cast<OpenglIndexBuffer*>(renderable->getIndexBuffer())->bind(m_ChunkVao.getId());
			static_cast<OpenglVertexBuffer*>(renderable->getVertexBuffer())->bind(m_ChunkVao.getId());
			glDrawElements(GL_TRIANGLES, renderable->getIndexBuffer()->elementsCount(), GL_UNSIGNED_INT, 0);
		}
		m_ChunksToRender.clear();
	}
}
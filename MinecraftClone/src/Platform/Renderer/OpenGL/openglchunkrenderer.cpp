#include "openglchunkrenderer.h"

#include <iostream>

namespace graphics {
	namespace opengl {

		ChunkRenderer::ChunkRenderData* OpenglChunkRenderer::createChunkRenderData() {
			m_Chunks.push_back(new chunkGraphicsComponent_t{ChunkRenderData(), OpenglVertexBuffer(&m_ChunkVao), OpenglIndexBuffer(&m_ChunkVao)});
			return &m_Chunks.back()->renderData;
		}

		void OpenglChunkRenderer::render(const Camera& camera) {
			m_ChunkShader.use();
			m_ChunkTextureAtlas.bind(0);
			m_ChunkVao.bind();
			for (auto& graphicsComponent : m_Chunks) {
				if (graphicsComponent->renderData.updated()) {
					graphicsComponent->vertices.setBuffer(graphicsComponent->renderData.getVertices().data(), 
						graphicsComponent->renderData.getVertices().size() * sizeof(chunkVertex_t), sizeof(chunkVertex_t));
					graphicsComponent->indices.setBuffer(graphicsComponent->renderData.getIndices().data(), 
						graphicsComponent->renderData.getIndices().size() * sizeof(uint32_t), sizeof(uint32_t));
					graphicsComponent->renderData.markFlushedToGPU();
				}

				graphicsComponent->indices.bind();
				graphicsComponent->vertices.bind();
				glDrawElements(GL_TRIANGLES, graphicsComponent->renderData.getIndices().size(), GL_UNSIGNED_INT, 0);
			}
		}

	}
}
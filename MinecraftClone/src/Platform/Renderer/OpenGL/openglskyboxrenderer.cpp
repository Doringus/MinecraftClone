#include "openglskyboxrenderer.h"

#include "../../../renderer/camera.h"
#include "openglgpubuffermanager.h"

namespace graphics::opengl {

	OpenglSkyboxRenderer::OpenglSkyboxRenderer() noexcept {
		m_GpuBufferManager = std::make_unique<OpenglGpuBufferManager>();
		m_Skybox = std::make_unique<RenderItem>(m_GpuBufferManager->createVertexBuffer(sizeof(skyboxVertex_t)), m_GpuBufferManager->createIndexBuffer());
		std::vector<skyboxVertex_t> vertices = {
			skyboxVertex_t{{1.0f,  1.0f,  1.0f}},
			skyboxVertex_t{{1.0f, -1.0f,  1.0f}},
			skyboxVertex_t{{1.0f,  1.0f, -1.0f}},
			skyboxVertex_t{{1.0f, -1.0f, -1.0f}},
			skyboxVertex_t{{-1.0f, -1.0f, -1.0f}},
			skyboxVertex_t{{-1.0f,  1.0f, -1.0f}},
			skyboxVertex_t{{-1.0f, -1.0f,  1.0f}},
			skyboxVertex_t{{-1.0f,  1.0f,  1.0f}} };
			
		
		std::vector<unsigned int> indices = { 
			0, 1, 3,
			3, 2, 0,
			0, 1, 7,
			7, 6, 1,
			1, 3, 6,
			6, 4, 3,
			3, 2, 4,
			4, 2, 5,
			5, 4, 6,
			6, 5, 7,
			7, 5, 2,
			2, 0, 7 };
		m_Skybox->getVertexBuffer()->appendData(vertices.size() * sizeof(skyboxVertex_t), vertices.data());
		m_Skybox->getIndexBuffer()->appendData(indices.size() * sizeof(unsigned int), indices.data());
		m_Skybox->getVertexBuffer()->copyFromShadowBuffer();
		m_Skybox->getIndexBuffer()->copyFromShadowBuffer();
	}
		

	void OpenglSkyboxRenderer::render(const Camera& camera) {
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		glDepthFunc(GL_LEQUAL);
		m_SkyboxShader.use();
		m_SkyboxTexture.bind(0);
		m_SkyboxVao.bind();
		m_SkyboxShader.setUniformMat4("vpMatrix", camera.getProjectionMatrix() * glm::mat4(glm::mat3(camera.getViewMatrix())));
		static_cast<OpenglIndexBuffer*>(m_Skybox->getIndexBuffer())->bind(m_SkyboxVao.getId());
		static_cast<OpenglVertexBuffer*>(m_Skybox->getVertexBuffer())->bind(m_SkyboxVao.getId());
		glDrawElements(GL_TRIANGLES, m_Skybox->getIndexBuffer()->elementsCount(), GL_UNSIGNED_INT, 0);
	}
}
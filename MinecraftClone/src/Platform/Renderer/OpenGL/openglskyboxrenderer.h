#pragma once

#include "../../../renderer/skyboxrenderer.h"
#include "base/openglinputlayout.h"
#include "base/openglvertexbuffer.h"
#include "base/openglindexbuffer.h"
#include "base/openglshader.h"
#include "base/openglcubemap.h"

namespace graphics::opengl {

	class OpenglSkyboxRenderer : public SkyboxRenderer {
	public:
		OpenglSkyboxRenderer() noexcept;

		void render(const Camera& camera) override;
	private:
		OpenglVertexBuffer m_SkyboxVertices = OpenglVertexBuffer(sizeof(skyboxVertex_t));
		OpenglInputLayout m_SkyboxVao = OpenglInputLayout({
					{"position", graphics::ShaderDataType::FLOAT3, 3}
		});
		OpenglShader m_SkyboxShader = { std::filesystem::absolute("src/shaders/skybox.vert"),
											   std::filesystem::absolute("src/shaders/skybox.frag") };
		OpenglCubemap m_SkyboxTexture = { {Image(std::filesystem::path("res\\skyboxRight.png"), 4), Image(std::filesystem::path("res\\skyboxLeft.png"), 4),
										   Image(std::filesystem::path("res\\skyboxTop.png"), 4), Image(std::filesystem::path("res\\skyboxBottom.png"), 4),
										   Image(std::filesystem::path("res\\skyboxFront.png"), 4), Image(std::filesystem::path("res\\skyboxBack.png"), 4)} };
	};

}
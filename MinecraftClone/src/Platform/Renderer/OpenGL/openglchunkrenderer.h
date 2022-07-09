#pragma once

#include <vector>

#include "../../../renderer/chunkrenderer.h"
#include "base/openglinputlayout.h"
#include "base/openglvertexbuffer.h"
#include "base/openglindexbuffer.h"
#include "base/openglshader.h"
#include "base/opengltexture.h"

namespace graphics {

	class Camera;

	namespace opengl {

		class OpenglChunkRenderer : public ChunkRenderer {
		public:
			OpenglChunkRenderer() noexcept;

			std::shared_ptr<RenderItem> createChunkRenderData() override;
			void submit(const std::shared_ptr<RenderItem>& chunk) override;
			void render(const Camera& camera) override;

		private:
			std::vector<std::shared_ptr<RenderItem>> m_ChunksToRender;

			OpenglInputLayout m_ChunkVao = OpenglInputLayout({
					{"position", graphics::ShaderDataType::FLOAT3, 3},
					{"texCoords", graphics::ShaderDataType::FLOAT2, 2}
					});
			OpenglShader m_ChunkShader		  = {  std::filesystem::absolute("src/shaders/default.vert"), 
												   std::filesystem::absolute("src/shaders/default.frag") };
			OpenglTexture m_ChunkTextureAtlas = { Image(std::filesystem::path("res\\texture.png"), 4) };
		};

	}

}
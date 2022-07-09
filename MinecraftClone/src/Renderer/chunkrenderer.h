#pragma once

#include <memory>
#include <vector>
#include <iterator>

#include "../../vendor/glm/vec3.hpp"
#include "../../vendor/glm/vec2.hpp"
#include "../../vendor/glm/mat4x4.hpp"

#include "igpubuffer.h"
#include "igpubuffermanager.h"

namespace graphics {

	struct chunkVertex_t {
		glm::vec3 position;
		glm::vec2 textureCoords;
	};

	

	class ChunkRenderData {
	public:
		void setModelMatrix(const glm::mat4& matrix) noexcept;
		glm::mat4 getModelMatrix() const noexcept;
	private:
		
	};
	
	class Camera;

	class ChunkRenderer {
	public:

		class ChunkRenderData {
		public:
			ChunkRenderData(std::unique_ptr<IGpuBuffer> vertexBuffer, std::unique_ptr<IGpuBuffer> indexBuffer);

			IGpuBuffer* getVertexBuffer() noexcept;
			IGpuBuffer* getIndexBuffer() noexcept;

			void setModelMatrix(const glm::mat4& matrix) noexcept;
			glm::mat4 getModelMatrix() const noexcept;

		private:
			std::unique_ptr<IGpuBuffer> m_VertexBuffer;
			std::unique_ptr<IGpuBuffer> m_IndexBuffer;
			glm::mat4 m_ModelMatrix;
		};

	public:
		virtual std::shared_ptr<ChunkRenderData> createChunkRenderData() = 0;
		virtual void submit(const std::shared_ptr<ChunkRenderData>& chunk) = 0;
		virtual void render(const Camera& camera) = 0;
	protected:
		std::unique_ptr<IGpuBufferManager> m_GpuBufferManager;
	};
}
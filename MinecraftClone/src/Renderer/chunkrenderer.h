#pragma once

#include <memory>
#include <vector>
#include <iterator>

#include "../../vendor/glm/vec3.hpp"
#include "../../vendor/glm/vec2.hpp"
#include "../../vendor/glm/mat4x4.hpp"

namespace graphics {

	struct chunkVertex_t {
		glm::vec3 position;
		glm::vec2 textureCoords;
	};

	class Camera;
	class IVertexBuffer;
	class IIndexBuffer;
	class ITexture;
	class IShader;
	class IInputLayout;

	class ChunkRenderer {
	public:

		class ChunkRenderData {
		public:
			bool updated() const noexcept;
			void beginChunk() noexcept;
			void markUpdated() noexcept;
			void markFlushedToGPU() noexcept;

			template<typename VBegin, typename VEnd, typename IBegin, typename IEnd>
			void addVertices(VBegin verticesBegin, VEnd verticesEnd, IBegin indicesBegin, IEnd indicesEnd) {
				std::copy(verticesBegin, verticesEnd, std::back_inserter(m_Vertices));
				std::copy(indicesBegin, indicesEnd, std::back_inserter(m_Indices));
			}
			
			std::vector<chunkVertex_t>& getVertices() noexcept;
			std::vector<uint32_t>& getIndices() noexcept;

			void setModelMatrix(const glm::mat4& matrix) noexcept;
			glm::mat4 getModelMatrix() const noexcept;

		private:
			bool m_IsUpdated = true;
			std::vector<chunkVertex_t> m_Vertices;
			std::vector<uint32_t> m_Indices;
			glm::mat4 m_ModelMatrix;
		};

	public:
		virtual ChunkRenderData* createChunkRenderData() = 0;
		virtual void render(const Camera& camera) = 0;
	};
}
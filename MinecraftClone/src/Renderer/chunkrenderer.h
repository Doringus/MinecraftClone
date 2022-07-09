#pragma once

#include <memory>
#include <vector>
#include <iterator>

#include "../../vendor/glm/vec3.hpp"
#include "../../vendor/glm/vec2.hpp"
#include "../../vendor/glm/mat4x4.hpp"

#include "igpubuffer.h"
#include "igpubuffermanager.h"
#include "renderitem.h"

namespace graphics {

	struct chunkVertex_t {
		glm::vec3 position;
		glm::vec2 textureCoords;
	};

	
	class Camera;

	class ChunkRenderer {
	public:
		virtual std::shared_ptr<RenderItem> createChunkRenderData() = 0;
		virtual void submit(const std::shared_ptr<RenderItem>& chunk) = 0;
		virtual void render(const Camera& camera) = 0;
	protected:
		std::unique_ptr<IGpuBufferManager> m_GpuBufferManager;
	};
}
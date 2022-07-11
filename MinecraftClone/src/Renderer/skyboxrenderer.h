#pragma once

#include <memory>

#include "../../vendor/glm/vec3.hpp"
#include "igpubuffermanager.h"
#include "renderitem.h"

namespace graphics {

	class Camera;

	struct skyboxVertex_t {
		glm::vec3 position;
	};

	class SkyboxRenderer {
	public:
		virtual ~SkyboxRenderer() = default;

		virtual void render(const Camera& camera) = 0;
	protected:
		std::unique_ptr<RenderItem> m_Skybox;
		std::unique_ptr<IGpuBufferManager> m_GpuBufferManager;
	};

}
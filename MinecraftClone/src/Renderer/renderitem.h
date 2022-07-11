#pragma once

#include <memory>

#include "../../vendor/glm/vec3.hpp"
#include "../../vendor/glm/vec2.hpp"
#include "../../vendor/glm/mat4x4.hpp"

#include "igpubuffer.h"

namespace graphics {

	class RenderItem {
	public:
		RenderItem(std::unique_ptr<IGpuBuffer> vertexBuffer, std::unique_ptr<IGpuBuffer> indexBuffer) noexcept;

		IGpuBuffer* getVertexBuffer() noexcept;
		IGpuBuffer* getIndexBuffer() noexcept;

		void setModelMatrix(const glm::mat4& matrix) noexcept;
		glm::mat4 getModelMatrix() const noexcept;

	private:
		std::unique_ptr<IGpuBuffer> m_VertexBuffer;
		std::unique_ptr<IGpuBuffer> m_IndexBuffer;
		glm::mat4 m_ModelMatrix = glm::mat4(1.0);
	};

}
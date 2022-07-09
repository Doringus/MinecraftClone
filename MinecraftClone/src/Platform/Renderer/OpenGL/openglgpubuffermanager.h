#pragma once

#include "../../../renderer/igpubuffermanager.h"

namespace graphics::opengl {

	class OpenglGpuBufferManager : public IGpuBufferManager {
	public:
		std::unique_ptr<IGpuBuffer> createVertexBuffer(size_t vertexSize) override;
		std::unique_ptr<IGpuBuffer> createIndexBuffer() override;
	};

}
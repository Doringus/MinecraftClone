#include "openglgpubuffermanager.h"

#include "base/openglindexbuffer.h"
#include "base/openglvertexbuffer.h"

namespace graphics::opengl {

	std::unique_ptr<IGpuBuffer> OpenglGpuBufferManager::createVertexBuffer(size_t vertexSize) {
		return std::make_unique<OpenglVertexBuffer>(vertexSize);
	}

	std::unique_ptr<IGpuBuffer> graphics::opengl::OpenglGpuBufferManager::createIndexBuffer() {
		return std::make_unique<OpenglIndexBuffer>();
	}
}



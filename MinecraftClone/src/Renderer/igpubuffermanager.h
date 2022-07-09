#pragma once

#include <memory>

namespace graphics {

	class IGpuBuffer;

	class IGpuBufferManager {
	public:
		virtual ~IGpuBufferManager() = default;

		virtual std::unique_ptr<IGpuBuffer> createVertexBuffer(size_t vertexSize) = 0;
		virtual std::unique_ptr<IGpuBuffer> createIndexBuffer() = 0;
	};

}
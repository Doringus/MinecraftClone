#pragma once

namespace graphics {

	class IGpuBufferManager {
	public:
		virtual ~IGpuBufferManager() = default;

		virtual std::unique_ptr<IGpuBuffer> createVertexBuffer() = 0;
		virtual std::unique_ptr<IGpuBuffer> createIndexBuffer() = 0;
	};

}
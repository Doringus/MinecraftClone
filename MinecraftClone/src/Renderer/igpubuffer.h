#pragma once

namespace graphics {

	class IGpuBuffer {
	public:
		virtual ~IGpuBuffer() = default;

		virtual size_t elementsCount() const noexcept = 0;
		virtual void* map() = 0;
		virtual void release() = 0;
		virtual void write(size_t size, void* data) = 0;
	};

}
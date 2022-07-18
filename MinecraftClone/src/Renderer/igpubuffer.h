#pragma once

#include "../rawbuffer.h"

namespace graphics {

	class IGpuBuffer {
	public:
		virtual ~IGpuBuffer() = default;

		virtual size_t elementsCount() const noexcept = 0;
		virtual void appendData(size_t size, void* data) = 0;
		virtual void clear() = 0;
		virtual void copyFromShadowBuffer() = 0;
	protected:
		bool m_IsUpdated = false;
		utils::RawBuffer m_ShadowBuffer;
	};

}
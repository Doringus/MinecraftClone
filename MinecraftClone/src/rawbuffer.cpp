#include "rawbuffer.h"

#include <cstdlib>
#include <cstring>

namespace utils {

	RawBuffer::RawBuffer(size_t capacity) : m_Capacity(capacity) {
		m_BasePtr = std::malloc(capacity);
	}

	RawBuffer::~RawBuffer() noexcept {
		std::free(m_BasePtr);
	}

	void RawBuffer::submitData(void* data, size_t size) {
		if (m_Size + size > m_Capacity) {
			m_Capacity = m_Size + size;
			auto mem = std::realloc(m_BasePtr, m_Capacity);
			m_BasePtr = mem;
		} 
		void* dest = (unsigned char*)m_BasePtr + m_Size;
		std::memcpy(dest , data, size);
		m_Size += size;
	}

	void RawBuffer::clear() {
		m_Size = 0;
	}

	void* RawBuffer::getData() const noexcept {
		return m_BasePtr;
	}

	size_t RawBuffer::getSize() const noexcept {
		return m_Size;
	}

}
#include "openglindexbuffer.h"

#include "openglinputlayout.h"
#include "debug.h"

namespace graphics::opengl {
		
	OpenglIndexBuffer::OpenglIndexBuffer() {
		glCreateBuffers(1, &m_Handle);
	}

	OpenglIndexBuffer::~OpenglIndexBuffer() {
		glDeleteBuffers(1, &m_Handle);
	}

	size_t OpenglIndexBuffer::elementsCount() const noexcept {
		return m_SizeInBytes / m_ElementSize;
	}

	void* OpenglIndexBuffer::map() {
		return glMapNamedBuffer(m_Handle, GL_WRITE_ONLY);
	}

	void OpenglIndexBuffer::release() {
		glUnmapNamedBuffer(m_Handle);
	}

	void OpenglIndexBuffer::write(size_t size, void* data) {
		m_SizeInBytes += size;
		glNamedBufferData(m_Handle, size, data, GL_DYNAMIC_DRAW);
	}

	void OpenglIndexBuffer::bind(GLuint vaoHandle) const {
		glVertexArrayElementBuffer(vaoHandle, m_Handle);
	}

}
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
		return m_ShadowBuffer.getSize() / m_ElementSize;
	}

	void OpenglIndexBuffer::appendData(size_t size, void* data) {
		m_IsUpdated = false;
		m_ShadowBuffer.submitData(data, size);
	}

	void OpenglIndexBuffer::clear() {
		m_ShadowBuffer.clear();
	}

	void OpenglIndexBuffer::copyFromShadowBuffer() {
		if (!m_IsUpdated) {
			glNamedBufferData(m_Handle, m_ShadowBuffer.getSize(), m_ShadowBuffer.getData(), GL_DYNAMIC_DRAW);
			m_IsUpdated = true;
		}
		
	}

	void OpenglIndexBuffer::bind(GLuint vaoHandle) const {
		glVertexArrayElementBuffer(vaoHandle, m_Handle);
	}

}
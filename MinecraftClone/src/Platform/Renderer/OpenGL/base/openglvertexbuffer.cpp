#include "openglvertexbuffer.h"

#include "openglinputlayout.h"

using namespace graphics::opengl;

OpenglVertexBuffer::OpenglVertexBuffer(size_t elementSize) : m_ElementSize(elementSize) {
	glCreateBuffers(1, &m_Handle);
}

OpenglVertexBuffer::~OpenglVertexBuffer() {
	glDeleteBuffers(1, &m_Handle);
}

size_t graphics::opengl::OpenglVertexBuffer::elementsCount() const noexcept {
	return m_SizeInBytes / m_ElementSize;
}


void OpenglVertexBuffer::appendData(size_t size, void* data) {
	m_IsUpdated = false;
	m_ShadowBuffer.submitData(data, size);
}

void OpenglVertexBuffer::clear() {
	m_ShadowBuffer.clear();
}

void OpenglVertexBuffer::copyFromShadowBuffer() {
	if (!m_IsUpdated) {
		glNamedBufferData(m_Handle, m_ShadowBuffer.getSize(), m_ShadowBuffer.getData(), GL_DYNAMIC_DRAW);
		m_IsUpdated = true;
	}
}

void OpenglVertexBuffer::bind(GLuint vaoHandle) const {
	glVertexArrayVertexBuffer(vaoHandle, 1, m_Handle, 0, m_ElementSize);
}

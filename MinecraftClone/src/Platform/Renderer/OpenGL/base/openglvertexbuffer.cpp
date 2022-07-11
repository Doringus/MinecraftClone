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

void* OpenglVertexBuffer::map() {
	return glMapNamedBuffer(m_Handle, GL_WRITE_ONLY);
}

void OpenglVertexBuffer::release() {
	glUnmapNamedBuffer(m_Handle);
}

void OpenglVertexBuffer::write(size_t size, void* data) {
	m_SizeInBytes += size;
	glNamedBufferData(m_Handle, size, data, GL_DYNAMIC_DRAW);
}

void OpenglVertexBuffer::bind(GLuint vaoHandle) const {
	glVertexArrayVertexBuffer(vaoHandle, 1, m_Handle, 0, m_ElementSize);
}

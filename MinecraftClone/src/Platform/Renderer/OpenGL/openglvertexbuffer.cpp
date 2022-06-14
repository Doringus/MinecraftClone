#include "openglvertexbuffer.h"

#include "openglinputlayout.h"

using namespace graphics::opengl;

OpenglVertexBuffer::OpenglVertexBuffer(OpenglInputLayout const* layout) : m_ElementSize(0), m_Layout(layout) {
	glCreateBuffers(1, &m_Handle);
}

OpenglVertexBuffer::~OpenglVertexBuffer() {
	glDeleteBuffers(1, &m_Handle);
}

void OpenglVertexBuffer::setBuffer(void const* data, size_t totalSize, size_t elementSize) {
	m_ElementSize = elementSize;
	glNamedBufferData(m_Handle, totalSize, data, GL_DYNAMIC_DRAW);
}

void* OpenglVertexBuffer::mapBuffer() {
	return glMapNamedBuffer(m_Handle, GL_WRITE_ONLY);
}

void OpenglVertexBuffer::unmapBuffer() {
	glUnmapNamedBuffer(m_Handle);
}

void OpenglVertexBuffer::bind() const {
	glVertexArrayVertexBuffer(m_Layout->getId(), 1, m_Handle, 0, m_ElementSize);
}

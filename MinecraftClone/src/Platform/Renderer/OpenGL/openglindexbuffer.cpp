#include "openglindexbuffer.h"

#include <GL/glew.h>

OpenglIndexBuffer::OpenglIndexBuffer(OpenglInputLayout& inputLayout) : m_Layout(inputLayout) {
	glCreateBuffers(1, &m_Id);
}

OpenglIndexBuffer::~OpenglIndexBuffer() {
	glDeleteBuffers(1, &m_Id);
}

void OpenglIndexBuffer::setBuffer(Buffer<uint32_t> buffer) {
	m_Buffer = std::move(buffer);
	glNamedBufferData(m_Id, m_Buffer.getSizeInBytes(), m_Buffer.get(), GL_STATIC_DRAW);
}

void OpenglIndexBuffer::bind() {
	glVertexArrayElementBuffer(m_Layout.getId(), m_Id);
}

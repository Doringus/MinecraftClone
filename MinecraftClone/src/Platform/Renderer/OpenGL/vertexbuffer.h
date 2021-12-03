#pragma once

#include "../../../renderer/vertexbuffer.h"

#include <GL/glew.h>

#include "openglinputlayout.h"

template <typename Vertex>
class OpenglVertexBuffer : public IVertexBuffer<Vertex> {
public:
	explicit OpenglVertexBuffer(OpenglInputLayout& layout) : m_Layout(layout) {
		glCreateBuffers(1, &m_Id);
	}

	~OpenglVertexBuffer() {
		glDeleteBuffers(1, &m_Id);
	}

	void setBuffer(Buffer<Vertex> buffer) override {
		m_Buffer = std::move(buffer);
		glNamedBufferData(m_Id, m_Buffer.getSizeInBytes(), m_Buffer.get(), GL_STATIC_DRAW);
	}

	void bind() override {
		glVertexArrayVertexBuffer(m_Layout.getId(), 1, m_Id, 0, sizeof(Vertex));
	}

private:
	OpenglInputLayout& m_Layout;
	Buffer<Vertex> m_Buffer;
	GLuint m_Id;
};
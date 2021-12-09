#pragma once

#include "../../../renderer/vertexbuffer.h"

#include <GL/glew.h>

#include "openglgeometrybuffer.h"
#include "openglinputlayout.h"

template <typename Vertex>
class OpenglVertexBuffer : public OpenglGeometryBuffer<Vertex> {
public:
	explicit OpenglVertexBuffer(OpenglInputLayout& layout) : m_Layout(layout) { }

	virtual ~OpenglVertexBuffer() = default;

	void bind() override {
		glVertexArrayVertexBuffer(m_Layout.getId(), 1, this->m_Id, 0, sizeof(Vertex));
	}

private:
	OpenglInputLayout& m_Layout;
};
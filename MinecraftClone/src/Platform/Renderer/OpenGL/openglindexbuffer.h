#pragma once

#include <cstdint>

#include "openglgeometrybuffer.h"
#include "openglinputlayout.h"


class OpenglIndexBuffer : public OpenglGeometryBuffer<GLuint> {
public:
	explicit OpenglIndexBuffer(OpenglInputLayout& inputLayout) : m_Layout(inputLayout) {}
	~OpenglIndexBuffer() = default;

	void bind() override {
		glVertexArrayElementBuffer(m_Layout.getId(), m_Id);
	}
private:
	OpenglInputLayout& m_Layout;
};
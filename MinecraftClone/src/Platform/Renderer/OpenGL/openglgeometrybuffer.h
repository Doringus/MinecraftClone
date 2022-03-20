#pragma once

#include "../../../renderer/igeometrybuffer.h"

#include <GL/glew.h>

template <typename T>
class OpenglGeometryBuffer : public IGeometryBuffer<T> {
public:
	explicit OpenglGeometryBuffer() {
		glCreateBuffers(1, &m_Id);
	}

	virtual ~OpenglGeometryBuffer() {
		glDeleteBuffers(1, &m_Id);
	}

	virtual void setData(const Buffer<T>& buffer) override {
		glNamedBufferData(m_Id, buffer.getSizeInBytes(), buffer.get(), GL_STATIC_DRAW);
	}

	virtual void bind() = 0;

protected:
	GLuint m_Id;
};
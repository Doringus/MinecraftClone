#pragma once

#include <memory>

#include "vertexbuffer.h"
#include "iinputlayout.h"

template <typename T>
class Mesh {
	Mesh() noexcept;
	~Mesh();

	IVertexBuffer<T>& getVertexBuffer() const noexcept;
	IInputLayout& getInputLayout() const noexcept;

private:
	IVertexBuffer<T>* m_VertexBuffer;
	IInputLayout* m_InputLayout;
};
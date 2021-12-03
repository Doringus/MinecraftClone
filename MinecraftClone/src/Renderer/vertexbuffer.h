#pragma once

#include "../buffer.h"

template<typename Vertex>
class IVertexBuffer {
public:
	IVertexBuffer() noexcept = default;
	virtual ~IVertexBuffer() = default;
	
	virtual void setBuffer(Buffer<Vertex> buffer) = 0;
	virtual void bind() = 0;
};
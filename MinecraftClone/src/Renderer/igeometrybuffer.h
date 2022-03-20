#pragma once

#include "../buffer.h"

template<typename T>
class IGeometryBuffer {
public:
	virtual ~IGeometryBuffer() = default;
	
	virtual void setData(const Buffer<T>& buffer) = 0;
	virtual void bind() = 0;
};
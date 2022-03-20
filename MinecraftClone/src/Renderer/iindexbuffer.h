#pragma once

#include "../buffer.h"

#include <cstdint>

class IIndexBuffer {
public:
	virtual ~IIndexBuffer() = default;

	virtual void setBuffer(Buffer<uint32_t> buffer) = 0;
	virtual void bind() = 0;
};
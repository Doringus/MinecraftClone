#pragma once

#include "../../../renderer/iindexbuffer.h"

#include <cstdint>

#include "openglinputlayout.h"


class OpenglIndexBuffer : public IIndexBuffer {
public:
	OpenglIndexBuffer(OpenglInputLayout& inputLayout);
	~OpenglIndexBuffer();

	void setBuffer(Buffer<uint32_t> buffer) override;
	void bind() override;
private:
	uint32_t m_Id;
	OpenglInputLayout& m_Layout;
	Buffer<uint32_t> m_Buffer;
};
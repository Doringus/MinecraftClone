#pragma once

#include <string>
#include <initializer_list>
#include <vector>

enum class ShaderDataType {
	FLOAT = 0,
	FLOAT2,
	FLOAT3,
	UNSIGNED_INT,
	UNSIGNED_INT2,
	UNSIGNED_INT3
};

constexpr std::size_t getShaderDataSize(ShaderDataType type) {
	switch (type) {
		case ShaderDataType::FLOAT: return 4;
		case ShaderDataType::FLOAT2: return 4 * 2;
		case ShaderDataType::FLOAT3: return 4 * 3;
		case ShaderDataType::UNSIGNED_INT: return 4;
		case ShaderDataType::UNSIGNED_INT2: return 4 * 2;
		case ShaderDataType::UNSIGNED_INT3: return 4 * 3;
		default: return 0;
	}
}

constexpr GLenum getShaderDataType(ShaderDataType type) {
	switch (type) {
		case ShaderDataType::FLOAT: return GL_FLOAT;
		case ShaderDataType::FLOAT2: return GL_FLOAT;
		case ShaderDataType::FLOAT3: return GL_FLOAT;
		case ShaderDataType::UNSIGNED_INT: return GL_UNSIGNED_INT;
		case ShaderDataType::UNSIGNED_INT2: return GL_UNSIGNED_INT;
		case ShaderDataType::UNSIGNED_INT3: return GL_UNSIGNED_INT;
		default: return 0;
	}
}

struct bufferElementDesc_t {
	std::string name;
	ShaderDataType type;
	std::size_t attributesCount;
	std::size_t size;

	bufferElementDesc_t(std::string elementName, ShaderDataType elementType, std::size_t count) :
		name(std::move(elementName)), type(elementType), attributesCount(count), size(getShaderDataSize(elementType)) {}
};

class BufferLayout {
public:
	BufferLayout(const std::initializer_list<bufferElementDesc_t>& elements) noexcept : m_Elements(elements) {}

	std::vector<bufferElementDesc_t>& getElementsRef() noexcept {
		return m_Elements;
	}
private:
	std::vector<bufferElementDesc_t> m_Elements;
};
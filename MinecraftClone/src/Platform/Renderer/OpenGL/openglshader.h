#pragma once

#include "../../../renderer/ishader.h"

#include <cstdint>
#include <string>
#include <unordered_map>

class OpenglShader : public IShader {
public:
	explicit OpenglShader(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);
	~OpenglShader();

	void bind() override;

	void setMat4(std::string_view name, const glm::mat4& matrix) override;
private:
	uint32_t m_Program;
	std::unordered_map<std::string, uint32_t> m_UniformsLocations;
};
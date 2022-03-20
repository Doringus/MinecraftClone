#pragma once

#include "../../vendor/glm/mat4x4.hpp"

#include <string_view>

class IShader {
public:
	virtual ~IShader() = default;

	virtual void bind() = 0;
	
	virtual void setMat4(std::string_view name, const glm::mat4& matrix) = 0;
};
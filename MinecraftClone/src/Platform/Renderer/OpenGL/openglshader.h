#pragma once

#include "../../../Renderer/shader.h"

class OpenGLShader : public IShader {
public:
	OpenGLShader(const std::filesystem::path& vertexShaderPath, const std::filesystem::path& fragmentShaderPath);

	bool create() noexcept override;
	void use() const noexcept override;
	void setMatrix(const Matrix4x4& matrix) noexcept override;
	void setTexture(ITexture const * texture) noexcept override;
};
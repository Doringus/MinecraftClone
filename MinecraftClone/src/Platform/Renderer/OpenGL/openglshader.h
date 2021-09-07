#pragma once

#include "../../../Renderer/shader.h"

#include <GL/glew.h>
#include <map>

class OpenGLShader : public IShader {
public:
	OpenGLShader(const std::filesystem::path& vertexShaderPath, const std::filesystem::path& fragmentShaderPath);

	bool create() noexcept override;
	void use() const noexcept override;
	void setMatrix(const Matrix4x4& matrix) noexcept override;
	void setTexture(const ITexture& texture) noexcept override;
private:
	GLuint m_ShaderId;
	std::map<GLint, std::string> m_UniformLocations;
};
#include "openglshader.h"

#include <GL/glew.h>

OpenGLShader::OpenGLShader(const std::filesystem::path& vertexShaderPath, const std::filesystem::path& fragmentShaderPath) :
								IShader(vertexShaderPath, fragmentShaderPath) {
	
}

bool OpenGLShader::create() noexcept {
	/// load shader code from files
	/*GLint result;
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		return false;
	}
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		return false;
	}
	shader->id = glCreateProgram();
	glAttachShader(shader->id, vertexShader);
	glAttachShader(shader->id, fragmentShader);
	glLinkProgram(shader->id);
	glGetProgramiv(shader->id, GL_LINK_STATUS, &result);
	if (!result) {
		return false;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader); */
	return true;
}

void OpenGLShader::use() const noexcept {

}

void OpenGLShader::setMatrix(const Matrix4x4 & matrix) noexcept {
}

void OpenGLShader::setTexture(ITexture const * texture) noexcept {
}

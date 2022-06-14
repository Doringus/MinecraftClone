#include "openglshader.h"

#include <fstream>
#include <stdexcept>

#include <spdlog/spdlog.h>

#include "../../../../utils.h"
#include "../../../../../vendor/glm/gtc/type_ptr.hpp"

using namespace graphics::opengl;

OpenglShader::OpenglShader(const std::string& vertexShaderCode, const std::string& fragmentShaderCode) : m_ProgramId(0) {
	GLint result;
	GLchar info[512];
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	auto vertexCstr = vertexShaderCode.c_str();
	glShaderSource(vertexShader, 1, &vertexCstr, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(vertexShader, 512, NULL, info);
		spdlog::critical("Vertex shader compilation error : {}", info);
	}
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	auto fragmentCstr = fragmentShaderCode.c_str();
	glShaderSource(fragmentShader, 1, &fragmentCstr, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, info);
		spdlog::critical("Fragment shader compilation error : {}", info);
	}
	GLuint shader = glCreateProgram();
	glAttachShader(shader, vertexShader);
	glAttachShader(shader, fragmentShader);
	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader, 512, NULL, info);
		spdlog::critical("Shader program linking error: {}", info);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	m_ProgramId = shader;

	/// Load all uniforms
	GLint count;
	GLint size; 
	GLenum type;

	const GLsizei bufSize = 32;
	GLchar name[bufSize];
	GLsizei length;
	glGetProgramiv(m_ProgramId, GL_ACTIVE_UNIFORMS, &count);
	for (size_t i = 0; i < count; ++i) {
		glGetActiveUniform(m_ProgramId, (GLuint)i, bufSize, &length, &size, &type, name);
		m_UniformLocations[std::string(name)] = glGetUniformLocation(m_ProgramId, name);
	}
}

OpenglShader::OpenglShader(const std::filesystem::path& vertexShaderPath, const std::filesystem::path& fragmentShaderPath) : 
	OpenglShader(utils::readEntireFileToStr(vertexShaderPath), utils::readEntireFileToStr(fragmentShaderPath)) {}

OpenglShader::~OpenglShader() noexcept {
	glDeleteProgram(m_ProgramId);
}

void OpenglShader::use() const {
	glUseProgram(m_ProgramId);
}

void OpenglShader::setUniformMat4(const std::string& name, const glm::mat4& matrix) {
	if (!m_UniformLocations.contains(name)) {
		throw std::invalid_argument("Invalid uniform name");
	}
	glUniformMatrix4fv(m_UniformLocations[name], 1, GL_FALSE, glm::value_ptr(matrix));
}

void OpenglShader::setUniform1i(const std::string& name, int64_t value) {
	if (!m_UniformLocations.contains(name)) {
		throw std::invalid_argument("Invalid uniform name");
	}
	glUniform1i(m_UniformLocations[name], value);
}

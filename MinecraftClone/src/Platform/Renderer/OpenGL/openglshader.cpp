#include "openglshader.h"

#include "../../../singleton.h"
#include "../../../filesystem.h"

OpenGLShader::OpenGLShader(const std::filesystem::path& vertexShaderPath, const std::filesystem::path& fragmentShaderPath) :
								IShader(vertexShaderPath, fragmentShaderPath) {
	
}

bool OpenGLShader::create() noexcept {
	GLint result;
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	std::string shaderCode = Singleton<Filesystem>::get().readFile(m_VertexShaderPath);
	const char *shaderCodePtr = shaderCode.c_str();
	glShaderSource(vertexShader, 1, &shaderCodePtr, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		return false;
	}
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	shaderCode = Singleton<Filesystem>::get().readFile(m_FragmentShaderPath);
	shaderCodePtr = shaderCode.c_str();
	glShaderSource(fragmentShader, 1, &shaderCodePtr, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		return false;
	}
	m_ShaderId = glCreateProgram();
	glAttachShader(m_ShaderId, vertexShader);
	glAttachShader(m_ShaderId, fragmentShader);
	glLinkProgram(m_ShaderId);
	glGetProgramiv(m_ShaderId, GL_LINK_STATUS, &result);
	if (!result) {
		return false;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// get all uniforms from shader
	GLint count, size;
	GLenum type;
	GLchar name[100];
	GLsizei length;

	glGetProgramiv(m_ShaderId, GL_ACTIVE_UNIFORMS, &count);
	for (GLint i = 0; i < count; ++i) {
		glGetActiveUniform(m_ShaderId, i, 100, &length, &size, &type, name);
		std::string stringName(name, length);
		GLint location = glGetUniformLocation(m_ShaderId, stringName.c_str());
		m_UniformLocations[location] = std::move(stringName);
	}

	return true;
}

void OpenGLShader::use() const noexcept {
	glUseProgram(m_ShaderId);
}
	
void OpenGLShader::setMatrix(const Matrix4x4& matrix) noexcept {
	
}

void OpenGLShader::setTexture(const ITexture& texture) noexcept {

}

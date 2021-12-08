#include "openglshader.h"

#include <GL/glew.h>

#include "../../../../vendor/glm/gtc/type_ptr.hpp"

#include <cstdint>
#include <iostream>

OpenglShader::OpenglShader(const std::string& vertexShaderCode, const std::string& fragmentShaderCode) {
	uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* ptrVertexCode = vertexShaderCode.c_str();
	glShaderSource(vertexShader, 1, &ptrVertexCode, NULL);
	glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* ptrFragmentCode = fragmentShaderCode.c_str();
    glShaderSource(fragmentShader, 1, &ptrFragmentCode, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    m_Program = glCreateProgram();
    glAttachShader(m_Program, vertexShader);
    glAttachShader(m_Program, fragmentShader);
    glLinkProgram(m_Program);
    glGetProgramiv(m_Program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_Program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Cache uniforms
    GLint count;
    glGetProgramiv(m_Program, GL_ACTIVE_UNIFORMS, &count);
    m_UniformsLocations.reserve(count);

    GLint size; 
    GLenum type; 
    const GLsizei bufSize = 32;
    GLchar name[bufSize]; 
    GLsizei length;

    for (std::size_t i = 0; i < count; ++i) {
        glGetActiveUniform(m_Program, (GLuint)i, bufSize, &length, &size, &type, name);
        GLuint location = glGetUniformLocation(m_Program, name);
        m_UniformsLocations.insert({ std::string(name, length), location });
    }

}

OpenglShader::~OpenglShader() {
    glDeleteProgram(m_Program);
}


void OpenglShader::bind() {

    glUseProgram(m_Program);
}

void OpenglShader::setMat4(std::string_view name, const glm::mat4& matrix) {
    glUniformMatrix4fv(m_UniformsLocations.at(name.data()), 1, GL_FALSE, glm::value_ptr(matrix));
}

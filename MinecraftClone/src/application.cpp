#include "application.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <array>

#include "platform/glfw/glfwwindow.h"
#include "platform/glfw/glfwinputmapper.h"
#include "platform/glfw/input.h"
#include "platform/renderer/opengl/vertexbuffer.h"

#include "renderer/renderer.h"
#include "renderer/bufferlayout.h"
#include "buffer.h"

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   ourColor = aColor;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, 1.0f);\n"
"}\n\0";

Application::Application() noexcept : m_IsRunning(false) {
    /* init glfw as main window lib */
    glfwInit();
    m_Window = std::make_shared<GlfwWindow>("window", 640, 480);
    m_Input = new Input(m_Window);
}

void Application::run() {
	m_IsRunning = true;

    glfwMakeContextCurrent(static_cast<GLFWwindow*>(m_Window->getNativeWindow()));
   // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glewExperimental = GL_TRUE;
    GLenum      err;
    if ((err = glewInit()) != GLEW_OK)
        std::cout << glewGetErrorString(err) << std::endl;

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    Buffer<vertex_t> verticesBuffer(static_cast<vertex_t*>(std::malloc(sizeof(vertex_t) * 3)), sizeof(vertex_t) * 3);
    verticesBuffer.pushBack({ {0.5f, -0.5f, 0.0f},  {1.f, 0.f, 0.f}, {} });
    verticesBuffer.pushBack({ {-0.5f, -0.5f, 0.0f},   {0.f, 1.f, 0.f}, {} });
    verticesBuffer.pushBack({ {0.0f,  0.5f, 0.0f}, {0.f, 0.f, 1.f}, {} });

    BufferLayout bufferLayout = { 
        {"position", ShaderDataType::FLOAT3, 3},
        {"color", ShaderDataType::FLOAT3, 3}
    };
    OpenglInputLayout layout(bufferLayout);
    OpenglVertexBuffer<vertex_t> vertexBuffer(layout);
    vertexBuffer.setBuffer(std::move(verticesBuffer));


    glUseProgram(shaderProgram);

    while (m_Window->isOpen()) {
        m_Input->update();
        if (m_Input->isKeyPressed(GameKey::KEY_A)) {
            std::cout << "A pressed\n";
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        layout.bind();
        vertexBuffer.bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);

        m_Window->swapBuffers();
        m_Window->pollEvents();
    }

    m_IsRunning = false;

    glDeleteProgram(shaderProgram);

    glfwTerminate();
}

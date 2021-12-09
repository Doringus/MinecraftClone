#include "application.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <array>


#include "renderer/renderer.h"
#include "renderer/bufferlayout.h"
#include "buffer.h"

#include "platform/glfw/glfwwindow.h"
#include "platform/glfw/glfwinputmapper.h"
#include "platform/glfw/input.h"
#include "platform/renderer/opengl/vertexbuffer.h"
#include "platform/renderer/opengl/openglrenderer.h"
#include "platform/renderer/opengl/openglshader.h"
#include "platform/renderer/opengl/openglindexbuffer.h"

#include "../vendor/glm/gtc/matrix_transform.hpp"

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"uniform mat4 mvp;\n"
"void main()\n"
"{\n"
"   gl_Position = mvp * vec4(aPos, 1.0);\n"
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

    Buffer<vertex_t> verticesBuffer(static_cast<vertex_t*>(std::malloc(sizeof(vertex_t) * 3)), sizeof(vertex_t) * 3);
    verticesBuffer.pushBack({ {0.5f, -0.5f, 0.0f},  {1.f, 0.f, 0.f}, {} });
    verticesBuffer.pushBack({ {-0.5f, -0.5f, 0.0f},   {0.f, 1.f, 0.f}, {} });
    verticesBuffer.pushBack({ {0.0f,  0.5f, 0.0f}, {0.f, 0.f, 1.f}, {} });

    Buffer<uint32_t> indices(static_cast<uint32_t*>(std::malloc(sizeof(uint32_t) * 3)), sizeof(uint32_t) * 3);
    indices.pushBack(0);
    indices.pushBack(1);
    indices.pushBack(2);

    BufferLayout bufferLayout = { 
        {"position", ShaderDataType::FLOAT3, 3},
        {"color", ShaderDataType::FLOAT3, 3}
    };
    OpenglInputLayout layout(bufferLayout);
    OpenglVertexBuffer<vertex_t> vertexBuffer(layout);
    OpenglIndexBuffer indexBuffer(layout);

    vertexBuffer.setBuffer(std::move(verticesBuffer));
    indexBuffer.setBuffer(std::move(indices));
    OpenglShader shader(vertexShaderSource, fragmentShaderSource);

    shader.bind();
    IRenderer* renderer = new OpenglRenderer();

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection = glm::perspective(45.0f, 640.f / 480.f, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 2.0), glm::vec3(0.0, 0.0, -2.0) + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    shader.setMat4("mvp", projection * view * model);
    while (m_Window->isOpen()) {
        m_Input->update();
        if (m_Input->isKeyPressed(GameKey::KEY_A)) {
            std::cout << "A pressed\n";
        }
        renderer->beginFrame();

        layout.bind();
        vertexBuffer.bind();
        indexBuffer.bind();
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        m_Window->swapBuffers();
        m_Window->pollEvents();
    }

    m_IsRunning = false;

    delete renderer;

    glfwTerminate();
}

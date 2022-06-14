#include "application.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <array>

#include "platform/glfw/glfwwindow.h"
#include "platform/glfw/glfwinputmapper.h"
#include "platform/glfw/input.h"

#include "platform/renderer/opengl/openglvertexbuffer.h"
#include "platform/renderer/opengl/openglinputlayout.h"
#include "platform/renderer/opengl/openglshader.h"
#include "platform/renderer/opengl/opengltexture.h"
#include "platform/renderer/opengl/openglchunkrenderer.h"

#include "renderer/bufferlayout.h"
#include "renderer/camera.h"

#include "utils.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../vendor/stb/stb.h"

#include <spdlog/spdlog.h>

#include "../vendor/glm/gtc/matrix_transform.hpp"


Application::Application() noexcept : m_IsRunning(false) {
    /* init glfw as main window lib */
    glfwInit();
    m_Window = std::make_shared<GlfwWindow>("window", 640, 480);
    m_Input = new Input(m_Window);
    stbi_set_flip_vertically_on_load(1);
}

void Application::run() {
	m_IsRunning = true;
    spdlog::info("Application started");
    glfwMakeContextCurrent(static_cast<GLFWwindow*>(m_Window->getNativeWindow()));
   // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glewExperimental = GL_TRUE;
    GLenum      err;
    if ((err = glewInit()) != GLEW_OK)
        std::cout << glewGetErrorString(err) << std::endl;

    graphics::opengl::OpenglChunkRenderer renderer;
    auto chunk = renderer.createChunkRenderData();
    chunk->beginChunk();

    chunk->addVertices({
        {{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
        {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{1.0f, 0.0f, 0.0f}, {0.2f, 0.0f}},
        {{1.0f, 1.0f, 0.0f}, {0.2f, 1.0f}}
        }, {0, 1, 3, 1, 2, 3});

    graphics::Camera camera(glm::perspective(45.0f, (GLfloat)640 / (GLfloat)480, 0.1f, 100.0f));

    while (m_Window->isOpen()) {
        m_Input->update();
        camera.update(*m_Input);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        renderer.render(camera);

        m_Window->swapBuffers();
        m_Window->pollEvents();
    }

    m_IsRunning = false;

    glfwTerminate();
}

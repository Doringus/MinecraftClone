#include "application.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <array>

#include "platform/glfw/glfwwindow.h"
#include "platform/glfw/glfwinputmapper.h"
#include "platform/glfw/input.h"

#include "platform/renderer/opengl/openglchunkrenderer.h"
#include "platform/renderer/opengl/openglrenderercontext.h"

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

Application::~Application() noexcept {
    glfwTerminate();
}

void Application::run() {
	m_IsRunning = true;
    spdlog::info("Application started");
    
    graphics::opengl::OpenglRendererContext context;
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
    
    double dt = 1.0 / 60.0;
    double beginTicks = glfwGetTimerValue();

    while (m_Window->isOpen()) {
        m_Window->pollEvents();

        spdlog::info("Frame time {0}ms", dt * 1000);

        m_Input->update();
        camera.update(*m_Input, dt);
        context.clearScreen(0.2, 0.2, 0.2, 1.0);
        context.clearFramebuffer();
        renderer.render(camera);

        m_Window->swapBuffers();
        
        double endTicks = glfwGetTimerValue();
        dt = (endTicks - beginTicks) / (double)glfwGetTimerFrequency();

        if (dt > 1.0f) {
            dt = 1.0f / 60.0f;
        }
        beginTicks = endTicks;
    }

    m_IsRunning = false;
}

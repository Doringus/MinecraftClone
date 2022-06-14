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
    /*
    std::vector<vertex_t> verticesBuffer;
    verticesBuffer.push_back({ {0.5f, -0.5f, 0.0f},  {0.0f, 0.0f} });
    verticesBuffer.push_back({ {-0.5f, -0.5f, 0.0f},   {0.5f, 1.f} });
    verticesBuffer.push_back({ {0.0f,  0.5f, 0.0f}, {1.f, 0.f}});

    using namespace graphics::opengl;

    graphics::BufferLayout bufferLayout = {
        {"position", graphics::ShaderDataType::FLOAT3, 3},
        {"texCoords", graphics::ShaderDataType::FLOAT2, 2}
    };
    OpenglInputLayout layout(bufferLayout);
    OpenglVertexBuffer vertexBuffer(&layout);
    vertexBuffer.setBuffer(verticesBuffer.data(), verticesBuffer.size() * sizeof(vertex_t), sizeof(vertex_t));
    OpenglShader openglShader(std::filesystem::absolute("src/shaders/default.vert"), std::filesystem::absolute("src/shaders/default.frag"));
    openglShader.use();
    
    graphics::Image image(std::filesystem::path("res\\texture.png"), 4);
    OpenglTexture texture(image);
    texture.bind(0);
    */
    graphics::opengl::OpenglChunkRenderer renderer;
    auto chunk = renderer.createChunkRenderData();
    chunk->beginChunk();

    chunk->addVertices({
        {{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
        {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{1.0f, 0.0f, 0.0f}, {0.2f, 0.0f}},
        {{1.0f, 1.0f, 0.0f}, {0.2f, 1.0f}}
        }, {0, 1, 3, 1, 2, 3});
    while (m_Window->isOpen()) {
        m_Input->update();
        if (m_Input->isKeyPressed(GameKey::KEY_A)) {
            std::cout << "A pressed\n";
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        renderer.render(graphics::Camera());
      //  layout.bind();
       // vertexBuffer.bind();
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        m_Window->swapBuffers();
        m_Window->pollEvents();
    }

    m_IsRunning = false;

    glfwTerminate();
}

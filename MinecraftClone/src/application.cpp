#include "application.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <array>
#include <chrono>

#include "platform/glfw/glfwwindow.h"
#include "platform/glfw/glfwinputmapper.h"
#include "platform/glfw/input.h"

#include "platform/renderer/opengl/openglchunkrenderer.h"
#include "platform/renderer/opengl/openglrenderercontext.h"
#include "platform/renderer/opengl/openglskyboxrenderer.h"

#include "renderer/bufferlayout.h"
#include "renderer/camera.h"

#include "game/world/chunk.h"
#include "game/world/chunksmanager.h"
#include "game/world/dummyworldgenerator.h"
#include "threading/threadpool.h"

#include "utils.h"

#include <FastNoiseLite.h>

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

    using namespace std::chrono_literals;
    utils::ThreadPool tp(4);
    /*  tp.submit([]() {
        spdlog::info("Task 1 {0}", std::chrono::system_clock::now());
        std::this_thread::sleep_for(2000ms);
    });
    tp.submit([]() {
        spdlog::info("Task 2 {0}", std::chrono::system_clock::now());
        std::this_thread::sleep_for(2000ms);
        });
    tp.submit([]() {
        spdlog::info("Task 3 {0}", std::chrono::system_clock::now());
        std::this_thread::sleep_for(2000ms);
        });
    tp.submit([]() {
        spdlog::info("Task 4 {0}", std::chrono::system_clock::now());
        std::this_thread::sleep_for(2000ms);
        });
    tp.submit([]() {
        spdlog::info("Task 5 {0}", std::chrono::system_clock::now());
        std::this_thread::sleep_for(2000ms);
        });
    tp.submit([]() {
        spdlog::info("Task 6 {0}", std::chrono::system_clock::now());
        std::this_thread::sleep_for(2000ms);
        });
    tp.submit([]() {
        spdlog::info("Task 7 {0}", std::chrono::system_clock::now());
        std::this_thread::sleep_for(2000ms);
        });
        */
    graphics::opengl::OpenglRendererContext context;
    graphics::opengl::OpenglChunkRenderer renderer;
    graphics::opengl::OpenglSkyboxRenderer skyboxRenderer;

    game::world::BlocksMap blocksMap;
    game::world::blockTextureFormat_t tree {
    game::world::blockFaceTextureFormat_t{{0.0f, 1.0f}, {0.2f, 1.0f}, {0.2f, 0.0f}, {0.0f, 0.0f}}, // front
    game::world::blockFaceTextureFormat_t{{0.0f, 1.0f}, {0.2f, 1.0f}, {0.2f, 0.0f}, {0.0f, 0.0f}}, // back
    game::world::blockFaceTextureFormat_t{{0.2f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f}, {0.2f, 0.0f}}, // left
    game::world::blockFaceTextureFormat_t{{0.2f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f}, {0.2f, 0.0f}}, // right
    game::world::blockFaceTextureFormat_t{{0.2f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f}, {0.2f, 0.0f}}, //top
    game::world::blockFaceTextureFormat_t{{0.2f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f}, {0.2f, 0.0f}} // bottom
    };
    game::world::blockTextureFormat_t grass {
    game::world::blockFaceTextureFormat_t{{0.4f, 1.0f}, {0.6f, 1.0f}, {0.6f, 0.0f}, {0.4f, 0.0f}}, // front
    game::world::blockFaceTextureFormat_t{{0.4f, 1.0f}, {0.6f, 1.0f}, {0.6f, 0.0f}, {0.4f, 0.0f}}, // back
    game::world::blockFaceTextureFormat_t{{0.6f, 1.0f}, {0.4f, 1.0f}, {0.4f, 0.0f}, {0.6f, 0.0f}}, // left
    game::world::blockFaceTextureFormat_t{{0.6f, 1.0f}, {0.4f, 1.0f}, {0.4f, 0.0f}, {0.6f, 0.0f}}, // right
    game::world::blockFaceTextureFormat_t{{0.4f, 1.0f}, {0.2f, 1.0f}, {0.2f, 0.0f}, {0.4f, 0.0f}}, //top
    game::world::blockFaceTextureFormat_t{{0.6f, 1.0f}, {0.4f, 1.0f}, {0.4f, 0.0f}, {0.6f, 0.0f}} // bottom
    };
    game::world::blockTextureFormat_t dirt {    
    game::world::blockFaceTextureFormat_t{{0.4f, 1.0f}, {0.6f, 1.0f}, {0.6f, 0.0f}, {0.4f, 0.0f}}, // front
    game::world::blockFaceTextureFormat_t{{0.4f, 1.0f}, {0.6f, 1.0f}, {0.6f, 0.0f}, {0.4f, 0.0f}}, // back
    game::world::blockFaceTextureFormat_t{{0.6f, 1.0f}, {0.4f, 1.0f}, {0.4f, 0.0f}, {0.6f, 0.0f}}, // left
    game::world::blockFaceTextureFormat_t{{0.6f, 1.0f}, {0.4f, 1.0f}, {0.4f, 0.0f}, {0.6f, 0.0f}}, // right
    game::world::blockFaceTextureFormat_t{{0.6f, 1.0f}, {0.4f, 1.0f}, {0.4f, 0.0f}, {0.6f, 0.0f}}, //top
    game::world::blockFaceTextureFormat_t{{0.6f, 1.0f}, {0.4f, 1.0f}, {0.4f, 0.0f}, {0.6f, 0.0f}} // bottom
    };
    blocksMap[1] = tree;
    blocksMap[2] = grass;
    blocksMap[3] = dirt;

    game::world::BlocksDatabase blockDatabase(blocksMap);
    game::world::WorldBox box({ -4, -4 }, 8, 1);
    game::world::ChunksManager chunksManager(box, blockDatabase, std::make_unique<game::world::DummyWorldGenerator>(game::world::DummyWorldGenerator::noiseConfig_t{ 1337, 1338, 1333 },
        game::world::DummyWorldGenerator::BiomesConfig{}), &renderer, &tp);
    graphics::Camera camera(glm::perspective(45.0f, (GLfloat)640 / (GLfloat)480, 0.1f, 100.0f), glm::vec3(0.0, 160.0, 0.0));
    
    double dt = 1.0 / 60.0;
    double beginTicks = glfwGetTimerValue();
    
    while (m_Window->isOpen()) {
        m_Window->pollEvents();

       // spdlog::info("Frame time {0}ms", dt * 1000);
     //   spdlog::info("x {0}, y{1}, z{2}",  camera.position().x, camera.position().y, camera.position().z);
        m_Input->update();
        camera.update(*m_Input, dt);
        chunksManager.update(camera.position().x, camera.position().z, dt);


        context.clearScreen(0.2, 0.2, 0.2, 1.0);
        context.clearFramebuffer();
        chunksManager.submitChunksToRenderer();

        renderer.render(camera);
        skyboxRenderer.render(camera);

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

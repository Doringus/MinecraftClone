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
#include "game/world/generators/dummy/dummyworldgenerator.h"
#include "game/world/generators/cliffs/cliffsworldgenerator.h"
#include "threading/threadpool.h"
#include "game/world/singleplayerchunksloader.h"
#include "game/player.h"

#include "utils.h"

#include <FastNoiseLite.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../vendor/stb/stb.h"

#include <spdlog/spdlog.h>

#include "../vendor/glm/gtc/matrix_transform.hpp"


Application::Application() noexcept : m_IsRunning(false) {
    /* init glfw as main window lib */
    glfwInit();
    m_Window = std::make_shared<GlfwWindow>("window", 1024, 1024);
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
  
    graphics::opengl::OpenglRendererContext context;
    graphics::opengl::OpenglChunkRenderer renderer;
    graphics::opengl::OpenglSkyboxRenderer skyboxRenderer;

    game::world::BlocksMap blocksMap;
    game::world::blockTextureFormat_t tree {
    game::world::blockFaceTextureFormat_t{{0.0f, 1.0f}, {0.2f, 1.0f}, {0.2f, 0.5f}, {0.0f, 0.5f}}, // front
    game::world::blockFaceTextureFormat_t{{0.0f, 1.0f}, {0.2f, 1.0f}, {0.2f, 0.5f}, {0.0f, 0.5f}}, // back
    game::world::blockFaceTextureFormat_t{{0.2f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.5f}, {0.2f, 0.5f}}, // left
    game::world::blockFaceTextureFormat_t{{0.2f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.5f}, {0.2f, 0.5f}}, // right
    game::world::blockFaceTextureFormat_t{{0.2f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.5f}, {0.2f, 0.5f}}, //top
    game::world::blockFaceTextureFormat_t{{0.2f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.5f}, {0.2f, 0.5f}} // bottom
    };
    game::world::blockTextureFormat_t grass {
    game::world::blockFaceTextureFormat_t{{0.4f, 1.0f}, {0.6f, 1.0f}, {0.6f, 0.5f}, {0.4f, 0.5f}}, // front
    game::world::blockFaceTextureFormat_t{{0.4f, 1.0f}, {0.6f, 1.0f}, {0.6f, 0.5f}, {0.4f, 0.5f}}, // back
    game::world::blockFaceTextureFormat_t{{0.6f, 1.0f}, {0.4f, 1.0f}, {0.4f, 0.5f}, {0.6f, 0.5f}}, // left
    game::world::blockFaceTextureFormat_t{{0.6f, 1.0f}, {0.4f, 1.0f}, {0.4f, 0.5f}, {0.6f, 0.5f}}, // right
    game::world::blockFaceTextureFormat_t{{0.4f, 1.0f}, {0.2f, 1.0f}, {0.2f, 0.5f}, {0.4f, 0.5f}}, //top
    game::world::blockFaceTextureFormat_t{{0.6f, 1.0f}, {0.4f, 1.0f}, {0.4f, 0.5f}, {0.6f, 0.5f}} // bottom
    };
    game::world::blockTextureFormat_t dirt {    
    game::world::blockFaceTextureFormat_t{{0.4f, 1.0f}, {0.6f, 1.0f}, {0.6f, 0.5f}, {0.4f, 0.5f}}, // front
    game::world::blockFaceTextureFormat_t{{0.4f, 1.0f}, {0.6f, 1.0f}, {0.6f, 0.5f}, {0.4f, 0.5f}}, // back
    game::world::blockFaceTextureFormat_t{{0.6f, 1.0f}, {0.4f, 1.0f}, {0.4f, 0.5f}, {0.6f, 0.5f}}, // left
    game::world::blockFaceTextureFormat_t{{0.6f, 1.0f}, {0.4f, 1.0f}, {0.4f, 0.5f}, {0.6f, 0.5f}}, // right
    game::world::blockFaceTextureFormat_t{{0.6f, 1.0f}, {0.4f, 1.0f}, {0.4f, 0.5f}, {0.6f, 0.5f}}, //top
    game::world::blockFaceTextureFormat_t{{0.6f, 1.0f}, {0.4f, 1.0f}, {0.4f, 0.5f}, {0.6f, 0.5f}} // bottom
    };
    game::world::blockTextureFormat_t stone{
    game::world::blockFaceTextureFormat_t{{0.6f, 1.0f}, {0.8f, 1.0f}, {0.8f, 0.5f}, {0.6f, 0.5f}}, // front
    game::world::blockFaceTextureFormat_t{{0.6f, 1.0f}, {0.8f, 1.0f}, {0.8f, 0.5f}, {0.6f, 0.5f}}, // back
    game::world::blockFaceTextureFormat_t{{0.8f, 1.0f}, {0.6f, 1.0f}, {0.6f, 0.5f}, {0.8f, 0.5f}}, // left
    game::world::blockFaceTextureFormat_t{{0.8f, 1.0f}, {0.6f, 1.0f}, {0.6f, 0.5f}, {0.8f, 0.5f}}, // right
    game::world::blockFaceTextureFormat_t{{0.8f, 1.0f}, {0.6f, 1.0f}, {0.6f, 0.5f}, {0.8f, 0.5f}}, //top
    game::world::blockFaceTextureFormat_t{{0.8f, 1.0f}, {0.6f, 1.0f}, {0.6f, 0.5f}, {0.8f, 0.5f}} // bottom
    };
    game::world::blockTextureFormat_t water{
    game::world::blockFaceTextureFormat_t{{0.8f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.5f}, {0.8f, 0.5f}}, // front
    game::world::blockFaceTextureFormat_t{{0.8f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.5f}, {0.8f, 0.5f}}, // back
    game::world::blockFaceTextureFormat_t{{1.0f, 1.0f}, {0.8f, 1.0f}, {0.8f, 0.5f}, {1.0f, 0.5f}}, // left
    game::world::blockFaceTextureFormat_t{{1.0f, 1.0f}, {0.8f, 1.0f}, {0.8f, 0.5f}, {1.0f, 0.5f}}, // right
    game::world::blockFaceTextureFormat_t{{1.0f, 1.0f}, {0.8f, 1.0f}, {0.8f, 0.5f}, {1.0f, 0.5f}}, //top
    game::world::blockFaceTextureFormat_t{{1.0f, 1.0f}, {0.8f, 1.0f}, {0.8f, 0.5f}, {1.0f, 0.5f}} // bottom
    };
    game::world::blockTextureFormat_t sand{
    game::world::blockFaceTextureFormat_t{{0.0f, 0.5f}, {0.2f, 0.5f}, {0.2f, 0.0f}, {0.0f, 0.0f}}, // front
    game::world::blockFaceTextureFormat_t{{0.0f, 0.5f}, {0.2f, 0.5f}, {0.2f, 0.0f}, {0.0f, 0.0f}}, // back
    game::world::blockFaceTextureFormat_t{{0.2f, 0.5f}, {0.0f, 0.5f}, {0.0f, 0.0f}, {0.2f, 0.0f}}, // left
    game::world::blockFaceTextureFormat_t{{0.2f, 0.5f}, {0.0f, 0.5f}, {0.0f, 0.0f}, {0.2f, 0.0f}}, // right
    game::world::blockFaceTextureFormat_t{{0.2f, 0.5f}, {0.0f, 0.5f}, {0.0f, 0.0f}, {0.2f, 0.0f}}, //top
    game::world::blockFaceTextureFormat_t{{0.2f, 0.5f}, {0.0f, 0.5f}, {0.0f, 0.0f}, {0.2f, 0.0f}} // bottom
    };
    game::world::blockTextureFormat_t snow{
    game::world::blockFaceTextureFormat_t{{0.2f, 0.5f}, {0.4f, 0.5f}, {0.4f, 0.0f}, {0.2f, 0.0f}}, // front
    game::world::blockFaceTextureFormat_t{{0.2f, 0.5f}, {0.4f, 0.5f}, {0.4f, 0.0f}, {0.2f, 0.0f}}, // back
    game::world::blockFaceTextureFormat_t{{0.4f, 0.5f}, {0.2f, 0.5f}, {0.2f, 0.0f}, {0.4f, 0.0f}}, // left
    game::world::blockFaceTextureFormat_t{{0.4f, 0.5f}, {0.2f, 0.5f}, {0.2f, 0.0f}, {0.4f, 0.0f}}, // right
    game::world::blockFaceTextureFormat_t{{0.4f, 0.5f}, {0.2f, 0.5f}, {0.2f, 0.0f}, {0.4f, 0.0f}}, //top
    game::world::blockFaceTextureFormat_t{{0.4f, 0.5f}, {0.2f, 0.5f}, {0.2f, 0.0f}, {0.4f, 0.0f}} // bottom
    };
    game::world::blockTextureFormat_t leaves{
    game::world::blockFaceTextureFormat_t{{0.4f, 0.5f}, {0.6f, 0.5f}, {0.6f, 0.0f}, {0.4f, 0.0f}}, // front
    game::world::blockFaceTextureFormat_t{{0.4f, 0.5f}, {0.6f, 0.5f}, {0.6f, 0.0f}, {0.4f, 0.0f}}, // back
    game::world::blockFaceTextureFormat_t{{0.6f, 0.5f}, {0.4f, 0.5f}, {0.4f, 0.0f}, {0.6f, 0.0f}}, // left
    game::world::blockFaceTextureFormat_t{{0.6f, 0.5f}, {0.4f, 0.5f}, {0.4f, 0.0f}, {0.6f, 0.0f}}, // right
    game::world::blockFaceTextureFormat_t{{0.6f, 0.5f}, {0.4f, 0.5f}, {0.4f, 0.0f}, {0.6f, 0.0f}}, //top
    game::world::blockFaceTextureFormat_t{{0.6f, 0.5f}, {0.4f, 0.5f}, {0.4f, 0.0f}, {0.6f, 0.0f}} // bottom
    };

    blocksMap[1] = tree;
    blocksMap[2] = grass;
    blocksMap[3] = dirt;
    blocksMap[4] = stone;
    blocksMap[5] = water;
    blocksMap[6] = sand;
    blocksMap[7] = snow;
    blocksMap[8] = leaves;

    game::world::BlocksDatabase blockDatabase(blocksMap);
    game::world::WorldBox box({ -3, -3 }, 10, 1);

    game::world::BiomesConfig biomesConfig;
    biomesConfig[0] = { {0.0, 1.0, 0.3, 0.7}, {2, 3, 4, 150} }; // plain
    biomesConfig[1] = { {0.0, 1.0, 0.7, 1.0}, {6, 6, 4, 150} }; // desert
    biomesConfig[2] = { {0.0, 1.0, 0.0, 0.3}, {7, 7, 4, 150} }; // snow
    /*
    auto chunksLoader = std::make_unique<game::world::SingleplayerChunksLoader>(std::make_unique<game::world::DummyWorldGenerator>(
        game::world::DummyWorldGenerator::noiseConfig_t{ 1337, 1338, 1333 },
        biomesConfig)); */
    std::vector<std::pair<float, float>> erosionFunction({ {-1.0, 1.05}, {-0.7, 1.08}, {-0.6, 1.07}, {0.2, 1.1}, {0.5, 1.09}, {0.7, 1.09}, {1.0, 1.1} }),
        pvFunction({ {-1.0, 1.2}, {-0.6, 1.14}, {0.2, 1.09}, {0.4, 1.05}, {0.7, 1.04}, {1.0, 0.1} });
    std::vector<std::pair<float, int>> continentalnessFunction({ {-1.0, 150}, {-0.7, 60}, {-0.5, 65}, {-0.4, 100}, {0.4, 150}, {1.0, 150} });
    game::world::CliffsWorldGenerator::noisesConfig_t noisesConfig{1337, 1567, 189, 2384, 1098, 10298};
    auto chunksLoader = std::make_unique<game::world::SingleplayerChunksLoader>(std::make_unique<game::world::CliffsWorldGenerator>(biomesConfig, noisesConfig, continentalnessFunction,
        erosionFunction, pvFunction));
    game::world::ChunksManager chunksManager(box, blockDatabase, std::move(chunksLoader), &renderer, &tp);
    graphics::Camera camera(glm::perspective(45.0f, (GLfloat)1024 / (GLfloat)1024, 0.1f, 500.0f));
    game::Player player(glm::vec3(0.0, 160.0, 0.0), &camera);

    double dt = 1.0 / 60.0;
    double beginTicks = glfwGetTimerValue();
    
    while (m_Window->isOpen()) {
        m_Window->pollEvents();

       // spdlog::info("Frame time {0}ms", dt * 1000);
        spdlog::info("x {0}, y{1}, z{2}",  camera.position().x, camera.position().y, camera.position().z);
        m_Input->update();
        player.update(*m_Input, dt);
        camera.update();
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

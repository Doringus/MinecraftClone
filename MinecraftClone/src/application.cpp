#include "application.h"

#include "Platform/glfwwindow.h"
#include "Platform/glfwinput.h"
#include "Platform/Renderer/OpenGL/openglrenderer.h"
#include "singleton.h"
#include "filesystem.h"

#include <iostream>

Application::Application() noexcept : m_Window(new GlfwWindow()), m_IsRunning(false) {
	m_Input = new GlfwInput(static_cast<GLFWwindow*>(m_Window->getNativeWindow()), std::make_unique<GlfwKeyWrapper>());
	m_Renderer = new OpenGLRenderer(m_Window);
}

Application::~Application() {
	delete m_Renderer;
	delete m_Window;
}


void Application::run() {
	m_IsRunning = true;
	std::filesystem::path p = "src/Shaders/basic.frag";
	std::cout << Singleton<Filesystem>::get().readFile(std::filesystem::absolute(p));
	
	while (m_Window->isOpen()) {
		m_Window->pollEvents();
		/// input
		m_Input->update();
		if (m_Input->isKeyPressed(GameKey::KEY_A)) {
			std::cout << "A key was pressed\n";
		}
		/// update
		/// render
		m_Renderer->beginFrame();
		m_Renderer->endFrame();
	}
}

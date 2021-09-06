#include "application.h"

#include "Platform/glfwwindow.h"
#include "Platform/glfwinput.h"
#include "Platform/Renderer/OpenGL/openglrenderingcontext.h"

#include <iostream>

Application::Application() noexcept : m_Window(new GlfwWindow()), m_IsRunning(false) {
	m_Input = new GlfwInput(static_cast<GLFWwindow*>(m_Window->getNativeWindow()), std::make_unique<GlfwKeyWrapper>());
	m_RenderingContext = new OpenGLRenderingContext(static_cast<GLFWwindow*>(m_Window->getNativeWindow()));
}

void Application::run() {
	m_IsRunning = true;
	m_RenderingContext->init();
	while (m_Window->isOpen()) {
		m_Window->pollEvents();
		/// input
		m_Input->update();
		if (m_Input->isKeyPressed(GameKey::KEY_A)) {
			std::cout << "A key was pressed\n";
		}
		/// update
		/// render
		m_RenderingContext->swapBuffers();
	}
}
#include "application.h"

#include "Platform/glfwwindow.h"
#include "Platform/glfwinput.h"

#include <iostream>

Application::Application() noexcept : m_Window(new GlfwWindow()), m_IsRunning(false) {
	m_Input = new GlfwInput(static_cast<GLFWwindow*>(m_Window->getNativeWindow()), std::make_unique<GlfwKeyWrapper>());
}

void Application::run() {
	m_IsRunning = true;
	while (m_Window->isOpen()) {
		m_Window->pollEvents();
		/// input
		m_Input->update();
		if (m_Input->isKeyPressed(GameKey::KEY_A)) {
			std::cout << "A key was pressed\n";
		}
		/// update
		/// render
	}
}
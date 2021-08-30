#include "application.h"

#include "Platform/glfwwindow.h"

Application::Application() noexcept : m_Window(new GlfwWindow()), m_IsRunning(false) {
}

void Application::run() {
	m_IsRunning = true;
	while (m_Window->isOpen()) {
		m_Window->pollEvents();
	}
}
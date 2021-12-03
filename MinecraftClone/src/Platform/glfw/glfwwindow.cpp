#include "glfwwindow.h"

#include <GLFW/glfw3.h>

GlfwWindow::GlfwWindow(const std::string& title, int32_t width, int32_t height) : IWindow(title, width, height) {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
}

GlfwWindow::~GlfwWindow() {
	glfwDestroyWindow(m_Window);
}

void* GlfwWindow::getNativeWindow() const noexcept {
	return static_cast<void*>(m_Window);
}

void GlfwWindow::pollEvents() const noexcept {
	glfwPollEvents();
}

bool GlfwWindow::isOpen() const noexcept {
	return !glfwWindowShouldClose(m_Window);
}

void GlfwWindow::swapBuffers() const noexcept {
	glfwSwapBuffers(m_Window);
}

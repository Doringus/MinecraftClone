#include "glfwwindow.h"

#include <iostream>

static bool glfwInitialized = false;

GlfwWindow::GlfwWindow(uint32_t width, uint32_t height, const std::string& title, RendererApi rendererApi) noexcept
	: Window(width, height, title, rendererApi) {

	if (!glfwInitialized) {
		if (!glfwInit()) {
			/// log error
			std::cout << "Error glfw init\n";
		}
		glfwInitialized = true;
	}

	if (rendererApi == RendererApi::OpenGL) {
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	} else if (rendererApi == RendererApi::DirectX11) {
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	}
	
	m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (m_Window == nullptr) {
		/// log error
		std::cout << "Error glfw window\n";
	}
	
	glfwSetWindowUserPointer(m_Window, this);

	auto resizeCallbackWrapper = [](GLFWwindow* window, int32_t width, int32_t height) {
		static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window))->windowResizeCallback(window, width, height);
	};
	auto framebufferResizeCallbackWrapper = [](GLFWwindow* window, int32_t width, int32_t height) {
		static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window))->framebufferResizeCallback(window, width, height);
	};
	
	glfwSetWindowSizeCallback(m_Window, resizeCallbackWrapper);
	glfwSetFramebufferSizeCallback(m_Window, framebufferResizeCallbackWrapper);

	glfwMakeContextCurrent(m_Window);
	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

GlfwWindow::~GlfwWindow() {
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

bool GlfwWindow::isOpen() const noexcept {
	return !glfwWindowShouldClose(m_Window);
}

void GlfwWindow::pollEvents() const {
	glfwPollEvents();
}

void GlfwWindow::resizeWindow(int32_t width, int32_t height) {
	glfwSetWindowSize(m_Window, width, height);
}

void * GlfwWindow::getNativeWindow() const noexcept {
	return m_Window;
}

void GlfwWindow::windowResizeCallback(GLFWwindow * window, int32_t width, int32_t height) {
	setWidth(width);
	setHeight(height);
	std::cout << "Resize\n";
}

void GlfwWindow::framebufferResizeCallback(GLFWwindow * window, int32_t widht, int32_t height) {
	std::cout << "Framebuffer resize\n";
}

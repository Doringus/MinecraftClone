#pragma once

#include "../window.h"
#include <GLFW/glfw3.h>

class GlfwWindow : public IWindow {
public:
	GlfwWindow(uint32_t width = 640, uint32_t height = 480, const std::string& title = "glfwWindow", RendererApi rendererApi = RendererApi::OpenGL) noexcept;
	virtual ~GlfwWindow();

	bool isOpen() const noexcept override;
	void pollEvents() const override;
	void resizeWindow(int32_t width, int32_t height) override;
	void* getNativeWindow() const noexcept override;

private:
	void windowResizeCallback(GLFWwindow* window, int32_t width, int32_t height);
	void framebufferResizeCallback(GLFWwindow* window, int32_t width, int32_t height);
private:
	GLFWwindow* m_Window;
};
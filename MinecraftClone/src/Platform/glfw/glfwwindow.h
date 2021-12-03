#pragma once

#include "../../iwindow.h"

#include <GLFW/glfw3.h>

class GlfwWindow : public IWindow {
public:
	explicit GlfwWindow(const std::string& title, int32_t width, int32_t height);
	~GlfwWindow();

	void* getNativeWindow() const noexcept override;
	void pollEvents() const noexcept override;
	bool isOpen() const noexcept override;
	void swapBuffers() const noexcept override;
private:
	GLFWwindow* m_Window;
};
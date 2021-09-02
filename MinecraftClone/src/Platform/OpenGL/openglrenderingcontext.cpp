#include "openglrenderingcontext.h"

#include <GLFW/glfw3.h>

OpenGLRenderingContext::OpenGLRenderingContext(GLFWwindow * window) noexcept : m_Window(window) {

}

void OpenGLRenderingContext::init() const {
	glfwMakeContextCurrent(m_Window);
}

void OpenGLRenderingContext::swapBuffers() const {
	glfwSwapBuffers(m_Window);
}

#include "openglrenderer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "openglshader.h"
#include "../../../window.h"

OpenGLRenderer::OpenGLRenderer(IWindow* window) noexcept :
							   IRenderer(window->getWidth(), window->getHeight()),
							   m_Window(window) {
	glewExperimental = GL_TRUE;
	if(!glewInit()) {
		/// error
	}
	glViewport(0, 0, m_Window->getWidth(), m_Window->getHeight());
	glfwMakeContextCurrent(static_cast<GLFWwindow*>(m_Window->getNativeWindow()));
}

void OpenGLRenderer::beginFrame() noexcept {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::push(batch_t& batch) noexcept {
	
}

void OpenGLRenderer::draw() const noexcept {
	
}

void OpenGLRenderer::endFrame() noexcept {
	glfwSwapBuffers(static_cast<GLFWwindow*>(m_Window->getNativeWindow()));
}
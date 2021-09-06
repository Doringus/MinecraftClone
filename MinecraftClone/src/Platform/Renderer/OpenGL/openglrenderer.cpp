#include "openglrenderer.h"

#include <GL/glew.h>

#include "openglshader.h"

OpenGLRenderer::OpenGLRenderer(uint32_t width, uint32_t height) noexcept : IRenderer(width, height) {

}

bool OpenGLRenderer::init() noexcept {
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		return false;
	}
	// TODO: create logs later
	///glEnable(GL_DEBUG_OUTPUT);
	if (m_RenderState.enableDepthTest) {
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_NEVER + static_cast<int>(m_RenderState.function));
	}
	glViewport(0, 0, m_ViewportWidth, m_ViewportHeight);
	
	return true;
}


void OpenGLRenderer::clearBuffers() const noexcept {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::drawIndexed(batch_t* batch) noexcept {
	setRenderState(batch->state);
	
	batch->shader->use();

	for (std::size_t i = 0; i < batch->vertexBuffers.size(); ++i) {

	}
}

void OpenGLRenderer::drawElements(batch_t* batch) noexcept {

}

void OpenGLRenderer::setRenderState(const renderState_t& renderState) noexcept {
	if (m_RenderState.enableDepthTest != renderState.enableDepthTest) {
		m_RenderState.enableDepthTest = renderState.enableDepthTest;
		if (renderState.enableDepthTest) {
			glEnable(GL_DEPTH_TEST);
		} else {
			glDisable(GL_DEPTH_TEST);
		}
	}

	if (m_RenderState.function != renderState.function) {
		m_RenderState.function = renderState.function;
		glDepthFunc(GL_NEVER + static_cast<int>(m_RenderState.function));
	}
	
}

#include "openglrenderer.h"

#include <GL/glew.h>

void OpenglRenderer::beginFrame() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void OpenglRenderer::submit() {

}

void OpenglRenderer::render() {

}

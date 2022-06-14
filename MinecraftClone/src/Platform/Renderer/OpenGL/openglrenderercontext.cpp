#include "openglrenderercontext.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>

namespace graphics {
	namespace opengl {

		OpenglRendererContext::OpenglRendererContext() {
			glewExperimental = GL_TRUE;
			GLenum      err;
			if ((err = glewInit()) != GLEW_OK) {
				spdlog::critical("Failed to init glew!!!");
			}
			glEnable(GL_DEPTH_TEST);
		}

		void OpenglRendererContext::clearScreen(float r, float g, float b, float alpha) {
			glClearColor(r, g, b, alpha);
		}

		void OpenglRendererContext::clearFramebuffer() {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

	}
}
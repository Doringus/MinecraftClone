#pragma once

#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../../../../renderer/image.h"

namespace graphics::opengl {

	class OpenglTexture {
	public:
		OpenglTexture(const Image& image);
		~OpenglTexture() noexcept;

		void bind(uint32_t slot) const;
	private:
		GLuint m_Id;
	};

}
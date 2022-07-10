#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../../../../renderer/image.h"
#include <vector>

namespace graphics::opengl {

	class OpenglCubemap {
	public:
		OpenglCubemap(const std::vector<Image>& images);
		~OpenglCubemap() noexcept;
		
		void bind(uint32_t slot);
	private:
		GLuint m_Id;
	};

}
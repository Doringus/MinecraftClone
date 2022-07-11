#include "openglcubemap.h"

namespace graphics::opengl {

	OpenglCubemap::OpenglCubemap(const std::vector<Image>& images) {
		glGenTextures(1, &m_Id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_Id);
		for (int i = 0; i < 6; ++i) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, images[i].width(), images[i].height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
				reinterpret_cast<const unsigned char*>(images[i].getImageData().data()));
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	OpenglCubemap::~OpenglCubemap() noexcept {
		glDeleteTextures(1, &m_Id);
	}

	void OpenglCubemap::bind(uint32_t slot) {
		glBindTextureUnit(slot, m_Id);
	}

}
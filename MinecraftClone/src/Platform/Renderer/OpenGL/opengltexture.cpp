#include "opengltexture.h"

#include "../../../renderer/image.h"

using namespace graphics::opengl;

OpenglTexture::OpenglTexture(const graphics::Image& image) {
	glCreateTextures(GL_TEXTURE_2D, 1, &m_Id);
	glTextureParameteri(m_Id, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_Id, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(m_Id, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTextureParameteri(m_Id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureStorage2D(m_Id, 1, GL_RGBA8, image.width(), image.height());
	glTextureSubImage2D(m_Id, 0, 0, 0, image.width(), image.height(), GL_RGBA, GL_UNSIGNED_BYTE, reinterpret_cast<const unsigned char*>(image.getImageData().data()));
	glGenerateTextureMipmap(m_Id);
}

OpenglTexture::~OpenglTexture() noexcept {
	glDeleteTextures(1, &m_Id);
}

void OpenglTexture::bind(uint32_t slot) const {
	glBindTextureUnit(slot, m_Id);
}

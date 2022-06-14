#include "image.h"

#include "../../vendor/stb/stb.h"

namespace graphics {

	Image::Image(const std::filesystem::path& path, uint32_t channels) {
		int32_t bpp;
		unsigned char* data = stbi_load(path.string().c_str(), &m_Width, &m_Height, &bpp, channels);
		m_Data.reserve(m_Width * m_Height * channels);
		for (size_t i = 0; i < m_Width * m_Height * channels; ++i) {
			m_Data.push_back(std::byte{ data[i] });
		}
		stbi_image_free(data);
	}
	int32_t Image::width() const noexcept {
		return m_Width;
	}
	int32_t Image::height() const noexcept {
		return m_Height;
	}
	std::vector<std::byte> const& Image::getImageData() const noexcept {
		return m_Data;
	}
}
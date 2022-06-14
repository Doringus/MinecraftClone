#pragma once

#include <filesystem>

namespace graphics {

	class Image {
	public:
		Image(const std::filesystem::path& path, uint32_t channels);

		int32_t width() const noexcept;
		int32_t height() const noexcept;

		std::vector<std::byte> const& getImageData() const noexcept;
	private:
		int32_t m_Width, m_Height;
		std::vector<std::byte> m_Data;
	};

}
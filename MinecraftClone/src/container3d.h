#pragma once

#include <vector>

namespace utils {

	template <typename T>
	class Container3d {
	public:
		Container3d(size_t width, size_t height, size_t depth) noexcept : m_Elements(width * height * depth), m_Width(width), m_Height(height), m_Depth(depth) { }

		const T& get(size_t x, size_t y, size_t z) const noexcept {
			return m_Elements.at(x + m_Width * (y + m_Height * z));
		}

		T& get(size_t x, size_t y, size_t z) noexcept {
			return m_Elements.at(x + m_Width * (y + m_Height * z));
		}

	private:
		std::vector<T> m_Elements;
		size_t m_Width, m_Height, m_Depth;
	};

}
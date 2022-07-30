#pragma once

#include <vector>

#include <boost/serialization/vector.hpp>

namespace utils {

	template <typename T>
	class Container3d {
	public:
		Container3d(size_t width, size_t height, size_t depth, bool shouldPreallocate = true) noexcept : m_Width(width), m_Height(height), m_Depth(depth) {
			if (shouldPreallocate) {
				m_Elements = std::vector<T>(width * height * depth);
			}
		}

		const T& get(size_t x, size_t y, size_t z) const noexcept {
			return m_Elements.at(x + m_Width * (y + m_Height * z));
		}

		T& get(size_t x, size_t y, size_t z) noexcept {
			return m_Elements.at(x + m_Width * (y + m_Height * z));
		}

		size_t size() const noexcept {
			return m_Elements.size();
		}

		T* data() noexcept {
			return m_Elements.data();
		}

	private:
		std::vector<T> m_Elements;
		size_t m_Width, m_Height, m_Depth;

	private:
		friend class boost::serialization::access;

		template <typename Archive>
		void serialize(Archive& ar, const unsigned int version) {
			ar & m_Elements;
		}
	};

}
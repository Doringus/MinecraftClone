#pragma once

#include <cstddef>

template <typename T>
class Buffer {
public:
	explicit Buffer(T* ptr = nullptr, std::size_t capacity = 0) noexcept: m_Data(ptr), m_Capacity(capacity), m_Size(0) {}
	Buffer(const Buffer& other) noexcept: m_Data(other.m_Data), m_Capacity(other.m_Capacity), m_Size(other.m_Size) {}
	Buffer(Buffer&& other) noexcept: m_Data(std::exchange(other.m_Data, nullptr)), 
							 m_Capacity(std::exchange(other.m_Capacity, 0)), 
							 m_Size(std::exchange(other.m_Size, 0)) {}

	~Buffer() = default;

	Buffer& operator=(Buffer& other) {
		if (this != &other) {
			m_Data = other.m_Data;
			m_Size = other.m_Size;
			m_Capacity = other.m_Capacity;
		}
		return *this;
	}

	Buffer& operator=(Buffer&& other) noexcept {
		if (this != &other) {
			m_Data = std::exchange(other.m_Data, nullptr);
			m_Capacity = std::exchange(other.m_Capacity, 0);
			m_Size = std::exchange(other.m_Size, 0);
		}
		return *this;
	}

	T* get() noexcept {
		return m_Data;
	}
	
	constexpr std::size_t count() const noexcept {
		return m_Size;
	}

	constexpr std::size_t getSizeInBytes() const noexcept {
		return m_Size * sizeof(T);
	}

	void pushBack(const T& data) {
		if (m_Size * sizeof(T) >= m_Capacity) {
			return;
		} 
		m_Data[m_Size] = data;
		m_Size++;
	}

	void pushBack(T&& data) {
		if (m_Size * sizeof(T) >= m_Capacity) {
			return;
		}
		m_Data[m_Size] = std::move(data);
		m_Size++;
	}
			

private:
	T* m_Data;
	std::size_t m_Size;
	std::size_t m_Capacity;
};
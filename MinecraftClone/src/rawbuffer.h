#pragma once

namespace utils {

	class RawBuffer {
	public:
		RawBuffer(size_t capacity = 512);
		~RawBuffer() noexcept;

		void submitData(void* data, size_t size);
		void clear();
		void* getData() const noexcept;
		size_t getSize() const noexcept;
	private:
		void* m_BasePtr = nullptr;
		size_t m_Size = 0, m_Capacity;
	};

}
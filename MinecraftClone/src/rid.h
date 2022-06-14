#pragma once

// resource id class  

#include <cstdint>

class RID {
public:

	constexpr RID(uint64_t id) noexcept : m_Id(id) { }

	uint64_t id() const noexcept {
		return m_Id;
	}

	auto operator<=>(const RID& other) const = default;

private:
	uint64_t m_Id;
};
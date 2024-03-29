#pragma once

#include "../../../vendor/glm/vec2.hpp"

#include <map>
#include <vector>

#include "blocks.h"

namespace game::world {

	struct blockFaceTextureFormat_t {
		glm::vec2 topLeft;
		glm::vec2 topRight;
		glm::vec2 bottomRight;
		glm::vec2 bottomLeft;
	};

	struct blockTextureFormat_t {
		blockFaceTextureFormat_t front, back;
		blockFaceTextureFormat_t left, right;
		blockFaceTextureFormat_t top, bottom;
	};

	using BlocksMap = std::map<uint16_t, blockTextureFormat_t>;

	class BlocksDatabase {
	public:
		BlocksDatabase(BlocksMap blocks) noexcept;
		BlocksDatabase() = default;
		BlocksDatabase(const BlocksDatabase& other) = default;
		BlocksDatabase(BlocksDatabase&& other) = default;

		blockTextureFormat_t const& getBlockTextureFormat(uint16_t blockType) const;

	private:
		BlocksMap m_Blocks;
	};

}
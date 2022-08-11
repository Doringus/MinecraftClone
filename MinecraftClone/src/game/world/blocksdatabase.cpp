#include "blocksdatabase.h"

namespace game::world {

	BlocksDatabase::BlocksDatabase(BlocksMap blocks) noexcept: m_Blocks(std::move(blocks)) {
		
	}

	blockTextureFormat_t const& BlocksDatabase::getBlockTextureFormat(uint16_t blockType) const {
		return m_Blocks.at(blockType);
	}

}
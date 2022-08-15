#include "blocksdatabase.h"

#include <spdlog/spdlog.h>

namespace game::world {

	BlocksDatabase::BlocksDatabase(BlocksMap blocks) noexcept: m_Blocks(std::move(blocks)) {
		
	}

	blockTextureFormat_t const& BlocksDatabase::getBlockTextureFormat(uint16_t blockType) const {
		if (!m_Blocks.contains(blockType)) {
			spdlog::info("NO TEXTURE");
		}
		return m_Blocks.at(blockType);
	}

}
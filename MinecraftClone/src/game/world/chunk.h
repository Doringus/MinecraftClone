#pragma once

#include <vector>

#include "../../renderer/chunkrenderer.h"
#include "blocksdatabase.h"
#include "../../container3d.h"

namespace game::world {
		
	constexpr int64_t get1DimChunkIndex(int64_t x, int64_t y, int64_t z, size_t height, size_t width) {
		return(z * height * width + y * width + x);
	}

	constexpr int64_t get1DimIndex(int64_t x, int64_t y, int64_t z, size_t dimensionSize) {
		return(z * dimensionSize * dimensionSize + y * dimensionSize + x);
	}

	struct chunkBox_t {
		int64_t xGrid;
		int64_t zGrid;
		size_t width;
		size_t height;
		size_t depth;
	};

	struct chunk_t {
		chunkBox_t box;
		utils::Container3d<uint16_t> blocks;
		std::shared_ptr<graphics::RenderItem> renderData;
	};
		
	void createChunkMesh(/*const World&,*/const BlocksDatabase& blocksDatabase, chunk_t& chunk);

}
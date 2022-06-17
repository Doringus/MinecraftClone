#pragma once

#include <vector>

#include "../../renderer/chunkrenderer.h"
#include "blocksdatabase.h"

namespace game {
	namespace world {

		constexpr int64_t get1DimIndex(int64_t x, int64_t y, int64_t z, size_t dimensionSize) {
			return(z * dimensionSize * dimensionSize + y * dimensionSize + x);
		}

		struct chunk_t {
			size_t size;
			int64_t xGrid;
			int64_t yGrid;
			graphics::ChunkRenderer::ChunkRenderData* renderData; /// non-owning ptr
			std::vector<uint16_t> blocks;
		};
		
		void createChunkMesh(/*const World&,*/const BlocksDatabase& blocksDatabase, chunk_t& chunk);

	}
}
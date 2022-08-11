#pragma once

#include <vector>

#include "../../renderer/chunkrenderer.h"
#include "blocksdatabase.h"
#include "../../container3d.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "blocks.h"

namespace game::world {
	
	class ChunksHashmapStorage;

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

		chunk_t(chunkBox_t box, utils::Container3d<BlockId> blocks, std::shared_ptr<graphics::RenderItem> renderItem) :
			box(std::move(box)), blocks(std::move(blocks)), renderData(std::move(renderItem)) {}

		chunkBox_t box;
		utils::Container3d<BlockId> blocks;
		std::shared_ptr<graphics::RenderItem> renderData;
		double m_TimeToLive = 10;

	private:
		friend class boost::serialization::access;

		template <typename Archive>
		void serialize(Archive& ar, const unsigned int version) {
			ar & blocks;
		}
	};
		
	void createChunkMesh(const ChunksHashmapStorage& storage, const BlocksDatabase& blocksDatabase, chunk_t& chunk);

}
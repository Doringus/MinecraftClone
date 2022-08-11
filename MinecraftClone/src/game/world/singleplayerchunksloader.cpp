#include "singleplayerchunksloader.h"

#include "generators/iworldgenerator.h"

#include <sstream>
#include <fstream>

#include <spdlog/spdlog.h>

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

namespace game::world {


	SingleplayerChunksLoader::SingleplayerChunksLoader(std::unique_ptr<IWorldGenerator> worldGenerator) : m_WorldGenerator(std::move(worldGenerator)) { }

	utils::Container3d<BlockId> SingleplayerChunksLoader::loadChunkBlocks(const chunkBox_t& box) {
		if (!m_StoredChunks.contains({box.xGrid, box.zGrid})) {
			return m_WorldGenerator->createChunk(box);
		} else {
			return loadChunkFromDisk(box);
		}
	}

	void SingleplayerChunksLoader::storeChunk(std::unique_ptr<chunk_t> chunk) {
		spdlog::info("Storing chunk {0}, {1} on disk", chunk->box.xGrid, chunk->box.zGrid);
		m_StoredChunks.emplace(chunk->box.xGrid, chunk->box.zGrid);
		std::stringstream fileName;
		fileName << "chunks/" << chunk->box.xGrid << "_" << chunk->box.zGrid << ".chunk";
		std::ofstream file(fileName.str(), std::ios::out | std::ios::binary);
		{
			boost::iostreams::filtering_streambuf<boost::iostreams::output> out;
			out.push(boost::iostreams::zlib_compressor(boost::iostreams::zlib::best_speed));
			out.push(file);
			boost::archive::binary_oarchive oa{ out };
			oa << chunk.get()->blocks;
		}	
	}

	utils::Container3d<BlockId> SingleplayerChunksLoader::loadChunkFromDisk(const chunkBox_t& box) {
		spdlog::info("Loading chunk {0}, {1} from disk", box.xGrid, box.zGrid);
		utils::Container3d<BlockId> result(16, 256, 16);
		std::stringstream fileName;
		fileName << "chunks/" << box.xGrid << "_" << box.zGrid << ".chunk";
		std::ifstream file{ fileName.str(), std::ios::in | std::ios::binary};
		{
			boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
			in.push(boost::iostreams::zlib_decompressor());
			in.push(file);
			boost::archive::binary_iarchive ia{ in };
			ia >> result;
		}
		return result;
	}

}
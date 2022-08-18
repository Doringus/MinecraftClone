#include "cliffsworldgenerator.h"

#include "../../chunk.h"
#include "../../blocks.h"

namespace game::world {

	CliffsWorldGenerator::CliffsWorldGenerator() {
		m_FractalWorldDensityGenerator = FastNoise::New<FastNoise::FractalFBm>();
		m_FractalWorldDensityGenerator->SetGain(0.5);
		auto fn = FastNoise::New<FastNoise::Perlin>();
		m_FractalWorldDensityGenerator->SetSource(fn);
	}

	utils::Container3d<uint16_t> CliffsWorldGenerator::createChunk(const chunkBox_t& chunk) const {
		utils::Container3d<uint16_t> result(16, 256, 16);
		utils::Container3d<float> densityContainer(16, 256, 16);
		std::vector<float> densityMap(16 * 256 * 16);
		m_FractalWorldDensityGenerator->GenUniformGrid3D(densityMap.data(), chunk.xGrid * 16, 0, chunk.zGrid * 16, 16, 256, 16, 0.02f, 1337);

		size_t index = 0;
		for (int64_t z = 0; z < chunk.depth; ++z) {
			for (int64_t y = 0; y < chunk.height; ++y) {
				for (int64_t x = 0; x < chunk.width; ++x) {
					densityContainer.get(x, y, z) = densityMap[index++];
				}
			}
		}
	
		float bias = 0.1;
		for (int64_t x = 0; x < chunk.width; ++x) {
			for (int64_t z = 0; z < chunk.depth; ++z) {
				for (int64_t y = 100; y >= 0; --y) {
					densityContainer.get(x, y, z) += bias;
					bias *= 1.8;
				}
				bias = 0.1;
			}
		}
		bias = 0.1;
		for (int64_t x = 0; x < chunk.width; ++x) {
			for (int64_t z = 0; z < chunk.depth; ++z) {
				for (int64_t y = 100; y < chunk.height; ++y) {
					densityContainer.get(x, y, z) -= bias;
					bias *= 1.05;
				}
				bias = 0.1;
			}
		}
		for (int64_t x = 0; x < chunk.width; ++x) {
			for (int64_t z = 0; z < chunk.depth; ++z) {
				for (int64_t y = 0; y < chunk.height; ++y) {
					float density = densityContainer.get(x, y, z);
					if (density > 0) {
						result.get(x, y, z) = blockTypeToRaw(BlockType::Stone);
					}
					else {
						result.get(x, y, z) = blockTypeToRaw(BlockType::Air);
					}
				}
			}
		}

		return result;
	}

}
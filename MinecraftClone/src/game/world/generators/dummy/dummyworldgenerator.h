#pragma once

#include "../iworldgenerator.h"
#include "../../chunk.h"
#include "../biomes.h"

#include <FastNoiseLite.h>

#include <unordered_map>

namespace game::world {
		
	class DummyWorldGenerator : public IWorldGenerator {
	public:

		struct noiseConfig_t {
			int humiditySeed;
			int temperatureSeed;
			int heightSeed;
		};

	public:
		DummyWorldGenerator(noiseConfig_t noiseConfig, BiomesConfig biomesConfig);

		utils::Container3d<BlockId> createChunk(const chunkBox_t& chunk) const override;
	private:
		void createColumn(utils::Container3d<BlockId>& blocks, float temperature, float humidity, float height, int x, int z) const;

	private:
		FastNoiseLite m_HumidityGenerator, m_TemperatureGenerator, m_HeightGenerator;
		BiomesConfig m_BiomesConfig;
	};

}
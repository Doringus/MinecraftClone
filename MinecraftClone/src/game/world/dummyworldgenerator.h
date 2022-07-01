#pragma once

#include "iworldgenerator.h"
#include "chunk.h"

#include <FastNoiseLite.h>

#include <unordered_map>

namespace game {
	namespace world {
		
		class DummyWorldGenerator : public IWorldGenerator {
		public:

			struct climateParams_t {
				double humidityThreshold;
				double temperatureThreshold;
			};
			
			struct biomeBlocksConfig_t {
				uint16_t groundBlock;
				uint16_t undergroundBlock;
				double undergroundLevel;
				double mountainLevel;
			};

			struct biomeConfig_t {
				climateParams_t climate;
				biomeBlocksConfig_t biomesBlocks;
			};

			using BiomesConfig = std::unordered_map<uint16_t, biomeConfig_t>;

			struct noiseConfig_t {
				int humiditySeed;
				int temperatureSeed;
				int heightSeed;
			};

		public:
			DummyWorldGenerator(noiseConfig_t noiseConfig, BiomesConfig biomesConfig);

			utils::Container3d<uint16_t> createChunk(const chunkBox_t& chunk) const override;
		private:
			void createColumn(utils::Container3d<uint16_t>& blocks, float temperature, float humidity, float height, int x, int z) const;

		private:
			FastNoiseLite m_HumidityGenerator, m_TemperatureGenerator, m_HeightGenerator;
			BiomesConfig m_BiomesConfig;
		};

	}
}
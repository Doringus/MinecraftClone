#pragma once

#include <FastNoise/FastNoise.h>

#include "../iworldgenerator.h"
#include "../biomes.h"

namespace game::world {

	class CliffsWorldGenerator : public IWorldGenerator {
	public:
		struct noisesConfig_t {
			int densitySeed;
			int continentalnessSeed, erosionSeed, pvSeed;
			int humiditySeed, temperatureSeed;
		};
	public:
		CliffsWorldGenerator(const BiomesConfig& biomesConfig, const noisesConfig_t& noisesConfig, const std::vector<std::pair<float, int>>& continentalnessFunction,
			const std::vector<std::pair<float, float>>& erosionFunction, const std::vector<std::pair<float, float>>& pvFunction);

		utils::Container3d<uint16_t> createChunk(const chunkBox_t& chunk) const override;
	private:
		void createColumn(utils::Container3d<uint16_t>& blocks, utils::Container3d<float>& density, int x, int z, 
			int height, float upSquash, float downSquash, float humidity, float temperature) const;
	private:
		FastNoise::SmartNode<FastNoise::FractalFBm> m_FractalWorldDensityGenerator { FastNoise::New<FastNoise::FractalFBm>() }, 
			m_ContinentalnessGenerator{ FastNoise::New<FastNoise::FractalFBm>() },
			m_ErosionGenerator{ FastNoise::New<FastNoise::FractalFBm>() }, m_PVGenerator{ FastNoise::New<FastNoise::FractalFBm>() };
		FastNoise::SmartNode<FastNoise::Perlin> m_HumidityGenerator { FastNoise::New<FastNoise::Perlin>() }, m_TemperatureGenerator { FastNoise::New<FastNoise::Perlin>() };
		BiomesConfig m_BiomesConfig;
		std::vector<std::pair<float, int>> m_ContinentalnessFunction;
		std::vector<std::pair<float, float>> m_ErosionFunction, m_PvFunction;
		noisesConfig_t m_NoisesConfig;
	};

}
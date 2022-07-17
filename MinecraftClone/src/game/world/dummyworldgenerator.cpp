#include "dummyworldgenerator.h"

#include <FastNoiseLite.h>
#include <spdlog/spdlog.h>

namespace game::world {

	DummyWorldGenerator::DummyWorldGenerator(noiseConfig_t noiseConfig, BiomesConfig biomesConfig) : m_BiomesConfig(std::move(biomesConfig)) {
		m_HumidityGenerator.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_OpenSimplex2);
		m_HumidityGenerator.SetSeed(noiseConfig.humiditySeed);
		m_HumidityGenerator.SetRotationType3D(FastNoiseLite::RotationType3D::RotationType3D_None);
		m_HumidityGenerator.SetFrequency(0.005);
		m_HumidityGenerator.SetFractalType(FastNoiseLite::FractalType::FractalType_FBm);
		m_HumidityGenerator.SetFractalOctaves(5);
		m_HumidityGenerator.SetFractalLacunarity(1.8);
		m_HumidityGenerator.SetFractalGain(0.3);
		m_HumidityGenerator.SetFractalWeightedStrength(0.3);

		m_TemperatureGenerator.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_OpenSimplex2);
		m_TemperatureGenerator.SetSeed(noiseConfig.temperatureSeed);
		m_TemperatureGenerator.SetRotationType3D(FastNoiseLite::RotationType3D::RotationType3D_None);
		m_TemperatureGenerator.SetFrequency(0.005);
		m_TemperatureGenerator.SetFractalType(FastNoiseLite::FractalType::FractalType_FBm);
		m_TemperatureGenerator.SetFractalOctaves(5);
		m_TemperatureGenerator.SetFractalLacunarity(1.8);
		m_TemperatureGenerator.SetFractalGain(0.3);
		m_TemperatureGenerator.SetFractalWeightedStrength(0.0);

		m_HeightGenerator.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_OpenSimplex2);
		m_HeightGenerator.SetSeed(noiseConfig.heightSeed);
		m_HeightGenerator.SetRotationType3D(FastNoiseLite::RotationType3D::RotationType3D_None);
		m_HeightGenerator.SetFrequency(0.003);
		m_HeightGenerator.SetFractalType(FastNoiseLite::FractalType::FractalType_FBm);
		m_HeightGenerator.SetFractalOctaves(5);
		m_HeightGenerator.SetFractalLacunarity(1.8);
		m_HeightGenerator.SetFractalGain(0.3);
		m_HeightGenerator.SetFractalWeightedStrength(0.3);
	}

	utils::Container3d<uint16_t> DummyWorldGenerator::createChunk(const chunkBox_t& chunk) const {
		utils::Container3d<uint16_t> result(16, 256, 16);
		for (int64_t x = 0; x < chunk.width; ++x) {
			for (int64_t z = 0; z < chunk.depth; ++z) {
				float temperature = const_cast<DummyWorldGenerator*>(this)->m_TemperatureGenerator.GetNoise<float>(x + chunk.xGrid, z + chunk.zGrid); /// why no const in fastnoise
				float humidity = const_cast<DummyWorldGenerator*>(this)->m_HumidityGenerator.GetNoise<float>(x + chunk.xGrid, z + chunk.zGrid); /// why no const in fastnoise
				float height = const_cast<DummyWorldGenerator*>(this)->m_HeightGenerator.GetNoise<float>((x + chunk.xGrid * int64_t(chunk.width)) / 10.0, 
					(z + chunk.zGrid * int64_t(chunk.depth)) / 10.0); /// why no const in fastnoise
				createColumn(result, temperature, humidity, height, x, z);
			}
		}
		return result;
	}

	void DummyWorldGenerator::createColumn(utils::Container3d<uint16_t>& blocks, float temperature, float humidity, float height, int x, int z) const {
		auto transformNoise = [](float noise) -> int {
			return static_cast<int>(((noise + 1.0) / 2.0) * 256);
		};
		int convertedHeight = transformNoise(height);
		for (int i = 0; i < 256; ++i) {
			if (i < convertedHeight) {
				blocks.get(x, i, z) = 1;
			} else {
				blocks.get(x, i, z) = 0;
			}
		}
	}

}
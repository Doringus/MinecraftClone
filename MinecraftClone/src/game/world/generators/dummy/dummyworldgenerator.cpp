#include "dummyworldgenerator.h"

#include <FastNoiseLite.h>
#include <spdlog/spdlog.h>

namespace game::world {

	DummyWorldGenerator::DummyWorldGenerator(noiseConfig_t noiseConfig, BiomesConfig biomesConfig) : m_BiomesConfig(std::move(biomesConfig)) {
		m_HumidityGenerator.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Cellular);
		m_HumidityGenerator.SetSeed(noiseConfig.humiditySeed);
		m_HumidityGenerator.SetRotationType3D(FastNoiseLite::RotationType3D::RotationType3D_None);
		m_HumidityGenerator.SetFrequency(0.011);
		m_HumidityGenerator.SetFractalType(FastNoiseLite::FractalType::FractalType_FBm);
		m_HumidityGenerator.SetFractalOctaves(2);
		m_HumidityGenerator.SetFractalLacunarity(1.8);
		m_HumidityGenerator.SetFractalGain(0.3);
		m_HumidityGenerator.SetFractalWeightedStrength(0.3);
		m_HumidityGenerator.SetCellularReturnType(FastNoiseLite::CellularReturnType::CellularReturnType_CellValue);

		m_TemperatureGenerator.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Cellular);
		m_TemperatureGenerator.SetSeed(noiseConfig.temperatureSeed);
		m_TemperatureGenerator.SetRotationType3D(FastNoiseLite::RotationType3D::RotationType3D_None);
		m_TemperatureGenerator.SetFrequency(0.011);
		m_TemperatureGenerator.SetFractalType(FastNoiseLite::FractalType::FractalType_FBm);
		m_TemperatureGenerator.SetFractalOctaves(2);
		m_TemperatureGenerator.SetFractalLacunarity(1.8);
		m_TemperatureGenerator.SetFractalGain(0.3);
		m_TemperatureGenerator.SetFractalWeightedStrength(0.0);
		m_TemperatureGenerator.SetCellularReturnType(FastNoiseLite::CellularReturnType::CellularReturnType_CellValue);

		m_HeightGenerator.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_OpenSimplex2);
		m_HeightGenerator.SetSeed(noiseConfig.heightSeed);
		m_HeightGenerator.SetRotationType3D(FastNoiseLite::RotationType3D::RotationType3D_None);
		m_HeightGenerator.SetFrequency(0.001);
		m_HeightGenerator.SetFractalType(FastNoiseLite::FractalType::FractalType_FBm);
		m_HeightGenerator.SetFractalOctaves(2);
		m_HeightGenerator.SetFractalLacunarity(1.8);
		m_HeightGenerator.SetFractalGain(0.8);
		m_HeightGenerator.SetFractalWeightedStrength(0.0);
	}

	utils::Container3d<BlockId> DummyWorldGenerator::createChunk(const chunkBox_t& chunk) const {
		utils::Container3d<BlockId> result(16, 256, 16);
		for (int64_t x = 0; x < chunk.width; ++x) {
			for (int64_t z = 0; z < chunk.depth; ++z) {
				float temperature = const_cast<DummyWorldGenerator*>(this)->m_TemperatureGenerator.GetNoise<float>(x + chunk.xGrid * int64_t(chunk.width), 
					z + chunk.zGrid * int64_t(chunk.depth)); /// why no const in fastnoise
				float humidity = const_cast<DummyWorldGenerator*>(this)->m_HumidityGenerator.GetNoise<float>(x + chunk.xGrid * int64_t(chunk.width), 
					z + chunk.zGrid * int64_t(chunk.depth)); /// why no const in fastnoise
				float height = const_cast<DummyWorldGenerator*>(this)->m_HeightGenerator.GetNoise<float>((x + chunk.xGrid * int64_t(chunk.width)), 
					(z + chunk.zGrid * int64_t(chunk.depth))); /// why no const in fastnoise
				createColumn(result, temperature, humidity, height, x, z);
			}
		}
		return result;
	}

	void DummyWorldGenerator::createColumn(utils::Container3d<BlockId>& blocks, float temperature, float humidity, float height, int x, int z) const {
		
		auto transform = [] (float value, float min, float max) -> float {
			float relative = (value + 1.0f) / 2.0f;
			return min + (max - min) * relative;
		};
		
		float convertedTemperature = transform(temperature, 0.f, 1.f);
		float convertedHumidity = transform(humidity, 0.f, 1.f);

		auto biome = std::find_if(m_BiomesConfig.begin(), m_BiomesConfig.end(), [convertedTemperature, convertedHumidity](const auto& b) {
			return b.second.climate.humidityFrom < convertedHumidity && b.second.climate.humidityTo > convertedHumidity &&
				   b.second.climate.temperatureFrom < convertedTemperature && b.second.climate.temperatureTo > convertedTemperature;
		});

		int convertedHeight = static_cast<int>(transform(height, 0.f, 256.f));
		int columnHeight = 0;

		const int worldHeight = 256;
		const int waterLevel = 70;

		for (int i = 0; i < worldHeight; ++i) {
			if (i < convertedHeight) {
				blocks.get(x, i, z) = BlockId::Stone;
			}
			else {
				if (columnHeight == 0) {
					columnHeight = i;
				}
				if (i < waterLevel) {
					blocks.get(x, i, z) = BlockId::Water;
				}
				else {
					blocks.get(x, i, z) = BlockId::Air;
				}
			}
		}
		if (biome != m_BiomesConfig.end()) {
			if (columnHeight < biome->second.biomesBlocks.mountainLevel) {
				blocks.get(x, columnHeight, z) = biome->second.biomesBlocks.groundBlock;
				for (int i = 1; i < biome->second.biomesBlocks.undergroundLevel; ++i) {
					blocks.get(x, columnHeight - i, z) = biome->second.biomesBlocks.undergroundBlock;
				}
			}
		}
	}

}
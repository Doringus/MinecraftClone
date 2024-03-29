#include "dummyworldgenerator.h"

#include <FastNoiseLite.h>
#include <spdlog/spdlog.h>

#include "../../blocks.h"

namespace game::world {

	DummyWorldGenerator::DummyWorldGenerator(noiseConfig_t noiseConfig, BiomesConfig biomesConfig) : m_BiomesConfig(std::move(biomesConfig)) {
		m_TreeGenerator.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Value);
		m_TreeGenerator.SetSeed(102);
		m_TreeGenerator.SetRotationType3D(FastNoiseLite::RotationType3D::RotationType3D_None);
		m_TreeGenerator.SetFrequency(0.111);
		m_TreeGenerator.SetFractalType(FastNoiseLite::FractalType::FractalType_FBm);
		m_TreeGenerator.SetFractalOctaves(2);
		m_TreeGenerator.SetFractalLacunarity(1.8);
		m_TreeGenerator.SetFractalGain(0.3);
		m_TreeGenerator.SetFractalWeightedStrength(0.3);

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

	utils::Container3d<uint16_t> DummyWorldGenerator::createChunk(const chunkBox_t& chunk) const {
		utils::Container3d<uint16_t> result(16, 256, 16);
		for (int64_t x = 0; x < chunk.width; ++x) {
			for (int64_t z = 0; z < chunk.depth; ++z) {
				float temperature = const_cast<DummyWorldGenerator*>(this)->m_TemperatureGenerator.GetNoise<float>(x + chunk.xGrid * int64_t(chunk.width), 
					z + chunk.zGrid * int64_t(chunk.depth)); /// why no const in fastnoise
				float humidity = const_cast<DummyWorldGenerator*>(this)->m_HumidityGenerator.GetNoise<float>(x + chunk.xGrid * int64_t(chunk.width), 
					z + chunk.zGrid * int64_t(chunk.depth)); /// why no const in fastnoise
				float height = const_cast<DummyWorldGenerator*>(this)->m_HeightGenerator.GetNoise<float>((x + chunk.xGrid * int64_t(chunk.width)), 
					(z + chunk.zGrid * int64_t(chunk.depth))); /// why no const in fastnoise
				float tree = const_cast<DummyWorldGenerator*>(this)->m_TreeGenerator.GetNoise<float>((x + chunk.xGrid * int64_t(chunk.width)),
					(z + chunk.zGrid * int64_t(chunk.depth))); /// why no const in fastnoise
				createColumn(result, temperature, humidity, height, tree, x, z);
			}
		}
		return result;
	}

	void DummyWorldGenerator::createColumn(utils::Container3d<uint16_t>& blocks, float temperature, float humidity, float height, float tree, int x, int z) const {
		
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
				blocks.get(x, i, z) = blockTypeToRaw(BlockType::Stone);
			}
			else {
				if (columnHeight == 0) {
					columnHeight = i;
				}
				if (i < waterLevel) {
					blocks.get(x, i, z) = blockTypeToRaw(BlockType::Water);
				}
				else {
					blocks.get(x, i, z) = blockTypeToRaw(BlockType::Air);
				}
			}
		}
		if (biome != m_BiomesConfig.end()) {
			if (columnHeight < biome->second.biomesBlocks.mountainLevel) {
				blocks.get(x, columnHeight, z) = biome->second.biomesBlocks.groundBlock;
				for (int i = 1; i < biome->second.biomesBlocks.undergroundLevel; ++i) {
					blocks.get(x, columnHeight - i, z) = biome->second.biomesBlocks.undergroundBlock;
				}
				if (tree > 0.65 && blocks.get(x, columnHeight, z) == blockTypeToRaw(BlockType::Grass)
					&& x < 11 && z < 11 && x >3 && z > 3) {
					placeTree(blocks, x, columnHeight, z);
				}
			}
		}
	}

	void DummyWorldGenerator::placeTree(utils::Container3d<uint16_t>& blocks, int x, int y, int z) const {
		for (int i = 0; i < 5; ++i) {
			blocks.get(x, y + i, z) = blockTypeToRaw(BlockType::Tree);
		}

		for (int i = -2; i < 5; ++i) {
			for (int j = -2; j < 5; ++j) {
				for (int k = 5; k < 7; ++k) {
					blocks.get(x + i, y + k, z + j) = blockTypeToRaw(BlockType::Leaves);
				}
			}
		}

		blocks.get(x, y + 7, z) = blockTypeToRaw(BlockType::Leaves);
		blocks.get(x, y + 8, z) = blockTypeToRaw(BlockType::Leaves);

	}

}
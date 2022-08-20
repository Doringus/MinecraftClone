#include "cliffsworldgenerator.h"

#include "../../chunk.h"
#include "../../blocks.h"

namespace game::world {

	CliffsWorldGenerator::CliffsWorldGenerator(const BiomesConfig& biomesConfig, const noisesConfig_t& noisesConfig, const std::vector<std::pair<float, int>>& continentalnessFunction,
		const std::vector<std::pair<float, float>>& erosionFunction, const std::vector<std::pair<float, float>>& pvFunction) : m_BiomesConfig(biomesConfig),
		m_ContinentalnessFunction(continentalnessFunction), m_ErosionFunction(erosionFunction), m_PvFunction(pvFunction), m_NoisesConfig(noisesConfig) {
		m_FractalWorldDensityGenerator->SetGain(0.5);
		m_FractalWorldDensityGenerator->SetSource(FastNoise::New<FastNoise::Perlin>());
		m_ContinentalnessGenerator->SetGain(0.3);
		m_ContinentalnessGenerator->SetSource(FastNoise::New<FastNoise::Perlin>());
		m_ErosionGenerator->SetGain(0.6);
		m_ErosionGenerator->SetSource(FastNoise::New<FastNoise::Perlin>());
		m_PVGenerator->SetGain(0.4);
		m_PVGenerator->SetSource(FastNoise::New<FastNoise::Perlin>());
	}

	utils::Container3d<uint16_t> CliffsWorldGenerator::createChunk(const chunkBox_t& chunk) const {
		utils::Container3d<uint16_t> result(chunk.width, chunk.height, chunk.depth);
		utils::Container3d<float> densityContainer(chunk.width, chunk.height, chunk.depth);
		std::vector<float> densityMap(chunk.width * chunk.height * chunk.depth), erosionMap(chunk.width * chunk.depth), continentalnessMap(chunk.width * chunk.depth), 
			pvMap(chunk.width * chunk.depth), humidityMap(chunk.width * chunk.depth), temperatureMap(chunk.width * chunk.depth);
		m_FractalWorldDensityGenerator->GenUniformGrid3D(densityMap.data(), chunk.xGrid * chunk.width, 0, chunk.zGrid * chunk.depth, 
			chunk.width, chunk.height, chunk.depth, 0.009f, m_NoisesConfig.densitySeed);
		m_ContinentalnessGenerator->GenUniformGrid2D(continentalnessMap.data(), chunk.xGrid * chunk.width, chunk.zGrid * chunk.depth,
			chunk.width, chunk.depth, 0.01f, m_NoisesConfig.continentalnessSeed);
		m_ErosionGenerator->GenUniformGrid2D(erosionMap.data(), chunk.xGrid * chunk.width, chunk.zGrid * chunk.depth,
			chunk.width, chunk.depth, 0.005f, m_NoisesConfig.erosionSeed);
		m_PVGenerator->GenUniformGrid2D(pvMap.data(), chunk.xGrid * chunk.width, chunk.zGrid * chunk.depth,
			chunk.width, chunk.depth, 0.005f, m_NoisesConfig.pvSeed);
		m_HumidityGenerator->GenUniformGrid2D(humidityMap.data(), chunk.xGrid * chunk.width, chunk.zGrid * chunk.depth,
			chunk.width, chunk.depth, 0.01f, m_NoisesConfig.humiditySeed);
		m_TemperatureGenerator->GenUniformGrid2D(temperatureMap.data(), chunk.xGrid * chunk.width,
			chunk.zGrid * chunk.depth, chunk.width, chunk.depth, 0.01f, m_NoisesConfig.temperatureSeed);

		size_t index = 0;
		for (int64_t z = 0; z < chunk.depth; ++z) {
			for (int64_t y = 0; y < chunk.height; ++y) {
				for (int64_t x = 0; x < chunk.width; ++x) {
					densityContainer.get(x, y, z) = densityMap[index++];
				}
			}
		}

		auto interpolator = [] <typename T>(const std::vector<std::pair<float, T>>& values, float x) {
			auto upper = std::find_if(values.begin(), values.end(), [x](const auto& element) {
				return x < element.first;
			});
			auto interpolation = [](float x1, T y1, float x2, T y2, float x) -> T {
				return y1 + (x - x1) * ((float)(y2 - y1) / (float)(x2 - x1));
			};
			return interpolation((upper - 1)->first, (upper - 1)->second, upper->first, upper->second, x);
		};

		for (int64_t x = 0; x < chunk.width; ++x) {
			for (int64_t z = 0; z < chunk.depth; ++z) {
				int height = interpolator(m_ContinentalnessFunction, continentalnessMap.at(z * chunk.depth + x));
				float upSquash = interpolator(m_PvFunction, pvMap.at(z * chunk.depth + x));
				float downSquash = interpolator(m_ErosionFunction, erosionMap.at(z * chunk.depth + x));
				createColumn(result,densityContainer, x, z, height, upSquash, downSquash, humidityMap.at(z * chunk.depth + x), temperatureMap.at(z * chunk.depth + x));
			}
		}

		return result;
	}

	void CliffsWorldGenerator::createColumn(utils::Container3d<uint16_t>& blocks, utils::Container3d<float>& density, int x, int z, 
											int height, float upSquash, float downSquash, float humidity, float temperature) const {
		float bias = 0.1;
		for (int64_t y = height; y >= 0; --y) {
			density.get(x, y, z) += bias;
			bias *= downSquash;
		}
		bias = 0.1;
		for (int64_t y = height; y < 256; ++y) {
			density.get(x, y, z) -= bias;
			bias *= upSquash;
		}
		for (int64_t y = 0; y < 256; ++y) {
			float d = density.get(x, y, z);
			if (d > 0) {
				blocks.get(x, y, z) = blockTypeToRaw(BlockType::Stone);
			}
			else {
				blocks.get(x, y, z) = blockTypeToRaw(BlockType::Air);
			}
		}

		const int waterLevel = 102;
		for (int i = 0; i < waterLevel; ++i) {
			if (blocks.get(x, i, z) == blockTypeToRaw(BlockType::Air)) {
				blocks.get(x, i, z) = blockTypeToRaw(BlockType::Water);
			}
		}

		auto transform = [](float value, float min, float max) -> float {
			float relative = (value + 1.0f) / 2.0f;
			return min + (max - min) * relative;
		};

		float convertedTemperature = transform(temperature, 0.f, 1.f);
		float convertedHumidity = transform(humidity, 0.f, 1.f);

		auto biome = std::find_if(m_BiomesConfig.begin(), m_BiomesConfig.end(), [convertedTemperature, convertedHumidity](const auto& b) {
			return b.second.climate.humidityFrom < convertedHumidity && b.second.climate.humidityTo > convertedHumidity &&
				b.second.climate.temperatureFrom < convertedTemperature&& b.second.climate.temperatureTo > convertedTemperature;
			});

		if (biome != m_BiomesConfig.end()) {
			for (int i = waterLevel; i < 256; ++i) {
				if (blocks.get(x, i, z) == blockTypeToRaw(BlockType::Air) &&
					blocks.get(x, i - 1, z) != blockTypeToRaw(BlockType::Air) && blocks.get(x, i - 1, z) != blockTypeToRaw(BlockType::Water)) {
					blocks.get(x, i - 1, z) = biome->second.biomesBlocks.groundBlock;
					for (int j = 2; j < biome->second.biomesBlocks.undergroundLevel; ++j) {
						blocks.get(x, i - j, z) = biome->second.biomesBlocks.undergroundBlock;
					}
				}
			}
		}
	}

}
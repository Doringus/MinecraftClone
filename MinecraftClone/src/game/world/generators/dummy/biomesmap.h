#pragma once

#include <vector>
#include <random>

namespace game::world {

	class IBiome;

	static std::random_device randomDevice;
	static std::default_random_engine randomEngine{ randomDevice };

	struct biomesMap_t {
		std::vector<uint8_t> biomesTable;
		size_t pixelSize;
		size_t size;
	};

	static void zoom(biomesMap_t& map) {
		std::vector<uint8_t> result(map.biomesTable.size() * 4);
		map.size <<= 2;
		std::uniform_int_distribution<int> uniform_dist(0, 1);
		for (size_t i = 0; i < map.size; ++i) {
			for (size_t j = 0; j < map.size; ++j) {
				int prob = uniform_dist(randomEngine);
				result[i + j * map.size] = prob == 0 ? 
					map.biomesTable[i / 2 + (j * map.size) / 2] : 0;
			}
		}

		map.biomesTable = std::move(result);
		map.pixelSize >>= 2;
		
	}
}
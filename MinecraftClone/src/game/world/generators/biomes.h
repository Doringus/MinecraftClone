#pragma once

#include <cstdint>
#include <unordered_map>

#include "../blocks.h"

namespace game::world {

	struct climateParams_t {
		double humidityFrom, humidityTo;
		double temperatureFrom, temperatureTo;
	};

	struct biomeBlocksConfig_t {
		BlockId groundBlock;
		BlockId undergroundBlock;
		uint16_t undergroundLevel;
		uint16_t mountainLevel;
	};

	struct biomeConfig_t {
		climateParams_t climate;
		biomeBlocksConfig_t biomesBlocks;
	};

	using BiomesConfig = std::unordered_map<uint16_t, biomeConfig_t>;

}
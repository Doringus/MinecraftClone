#pragma once

#include <FastNoise/FastNoise.h>

#include "../iworldgenerator.h"

namespace game::world {

	class CliffsWorldGenerator : public IWorldGenerator {
	public:
		CliffsWorldGenerator();

		utils::Container3d<uint16_t> createChunk(const chunkBox_t& chunk) const override;
	private:
		FastNoise::SmartNode<FastNoise::FractalFBm> m_FractalWorldDensityGenerator;
	};

}
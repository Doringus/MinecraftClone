#include "chunk.h"

#include <algorithm>

namespace game {
	namespace world {

		enum class QuadFace {
			Left,
			Right,
			Top,
			Bottom,
			Front,
			Back
		};

		void pushQuad(int32_t x, int32_t y, int32_t z, uint16_t type, QuadFace face, graphics::ChunkRenderer::ChunkRenderData* renderData) {
			renderData->beginChunk();

			 std::vector<graphics::chunkVertex_t> leftRightVertices = {
				{{0.0f + x + (face == QuadFace::Right ? 1 : 0), 1.0f + y, 1.0f + z}, {0.0f, 1.0f}}, // top-left front
				{{0.0f + x + (face == QuadFace::Right ? 1 : 0), 0.0f + y, 1.0f + z}, {0.0f, 0.0f}}, // bottom-left front
				{{0.0f + x + (face == QuadFace::Right ? 1 : 0), 0.0f + y, 0.0f + z}, {0.2f, 0.0f}}, // bottom-left back
				{{0.0f + x + (face == QuadFace::Right ? 1 : 0), 1.0f + y, 0.0f + z}, {0.2f, 1.0f}} // top-left back
			};
			std::vector<graphics::chunkVertex_t> bottomTopVertices = {
				{{0.0f + x, 0.0f + y + (face == QuadFace::Top ? 1 : 0), 1.0f + z}, {0.0f, 0.0f}}, // bottom-left front
				{{1.0f + x, 0.0f + y + (face == QuadFace::Top ? 1 : 0), 1.0f + z}, {0.2f, 0.0f}}, // bottom-right front
				{{1.0f + x, 0.0f + y + (face == QuadFace::Top ? 1 : 0), 0.0f + z}, {0.2f, 1.0f}}, // bottom-right back
				{{0.0f + x, 0.0f + y + (face == QuadFace::Top ? 1 : 0), 0.0f + z}, {0.0f, 1.0f}} // bottom-left back
			};
			std::vector<graphics::chunkVertex_t> backFrontVertices = {
				{{0.0f + x, 1.0f + y, 0.0f + z + (face == QuadFace::Front ? 1 : 0)}, {0.0f, 1.0f}}, // top-left back
				{{0.0f + x, 0.0f + y, 0.0f + z + (face == QuadFace::Front ? 1 : 0)}, {0.0f, 0.0f}}, // bottom-left back
				{{1.0f + x, 0.0f + y, 0.0f + z + (face == QuadFace::Front ? 1 : 0)}, {0.2f, 0.0f}}, // bottomright back
				{{1.0f + x, 1.0f + y, 0.0f + z + (face == QuadFace::Front ? 1 : 0)}, {0.2f, 1.0f}} // top-right back
			};

			std::vector<uint16_t> indicesTemplate = {
				0, 1, 3,
				1, 2, 3
			};
			std::for_each(indicesTemplate.begin(), indicesTemplate.end(), [renderData](uint16_t& value) {
				value += renderData->getVertices().size();
			});

			if (face == QuadFace::Top || face == QuadFace::Bottom) {
				renderData->addVertices(bottomTopVertices, indicesTemplate);
			}
			else if (face == QuadFace::Left || face == QuadFace::Right) {
				renderData->addVertices(leftRightVertices, indicesTemplate);
			}
			else if (face == QuadFace::Front || face == QuadFace::Back) {
				renderData->addVertices(backFrontVertices, indicesTemplate);
			}
		}

		void createChunkMesh(chunk_t& chunk) {
			/* Simple culling algorithm */
			for (size_t x = 0; x < chunk.size; ++x) {
				for (size_t y = 0; y < chunk.size; ++y) {
					for (size_t z = 0; z < chunk.size; ++z) {
						if (chunk.blocks[get1DimIndex(x, y, z, chunk.size)] == 0) {
							continue;
						}

						//left
						if (x > 0 && !chunk.blocks[get1DimIndex(x - 1, y, z, chunk.size)]) {
							pushQuad(x, y, z, 1, QuadFace::Left, chunk.renderData);
						}
						//right
						if (x < chunk.size - 1 && !chunk.blocks[get1DimIndex(x + 1, y, z, chunk.size)]) {
							pushQuad(x, y, z, 1, QuadFace::Right, chunk.renderData);
						}
						//top
						if (y < chunk.size - 1 && !chunk.blocks[get1DimIndex(x, y + 1, z, chunk.size)]) {
							pushQuad(x, y, z, 1, QuadFace::Top, chunk.renderData);
						}
						// bottom
						if (y > 0 && !chunk.blocks[get1DimIndex(x, y - 1, z, chunk.size)]) {
							pushQuad(x, y, z, 1, QuadFace::Bottom, chunk.renderData);
						}
						//front
						if (z < chunk.size - 1 && !chunk.blocks[get1DimIndex(x, y, z + 1, chunk.size)]) {
							pushQuad(x, y, z, 1, QuadFace::Front, chunk.renderData);
						}
						//back
						if (z > 0 && !chunk.blocks[get1DimIndex(x, y, z - 1, chunk.size)]) {
							pushQuad(x, y, z, 1, QuadFace::Back, chunk.renderData);
						}

					}
				}
			}
		}

	}
}
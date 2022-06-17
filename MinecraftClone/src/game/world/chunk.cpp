#include "chunk.h"

#include <algorithm>
#include <array>

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

		void addFrontFace(int32_t x, int32_t y, int32_t z, const blockFaceTextureFormat_t& textureFormat, graphics::ChunkRenderer::ChunkRenderData* renderData) {
			std::array<graphics::chunkVertex_t, 4> frontVertices = {
				graphics::chunkVertex_t{{0.0f + x, 1.0f + y, 1.0f + z}, textureFormat.topLeft}, // top-left front
				graphics::chunkVertex_t{{0.0f + x, 0.0f + y, 1.0f + z}, textureFormat.bottomLeft}, // bottom-left front
				graphics::chunkVertex_t{{1.0f + x, 0.0f + y, 1.0f + z}, textureFormat.bottomRight}, // bottom-right front
				graphics::chunkVertex_t{{1.0f + x, 1.0f + y, 1.0f + z}, textureFormat.topRight} // top-right front
			};

			std::array<uint16_t, 6> indicesTemplate = {
				0, 1, 3,
				1, 2, 3
			};
			std::for_each(indicesTemplate.begin(), indicesTemplate.end(), [renderData](uint16_t& value) {
				value += renderData->getVertices().size();
				});
			renderData->addVertices(frontVertices.begin(), frontVertices.end(), indicesTemplate.begin(), indicesTemplate.end());
		}

		void addBackFace(int32_t x, int32_t y, int32_t z, const blockFaceTextureFormat_t& textureFormat, graphics::ChunkRenderer::ChunkRenderData* renderData) {
			std::array<graphics::chunkVertex_t, 4> backVertices = {
				graphics::chunkVertex_t{{0.0f + x, 1.0f + y, 0.0f + z}, textureFormat.topLeft}, // top-left back
				graphics::chunkVertex_t{{0.0f + x, 0.0f + y, 0.0f + z}, textureFormat.bottomLeft}, // bottom-left back
				graphics::chunkVertex_t{{1.0f + x, 0.0f + y, 0.0f + z}, textureFormat.bottomRight}, // bottom-right back
				graphics::chunkVertex_t{{1.0f + x, 1.0f + y, 0.0f + z}, textureFormat.topRight} // top-right back
			};

			std::array<uint16_t, 6> indicesTemplate = {
				0, 1, 3,
				1, 2, 3
			};
			std::for_each(indicesTemplate.begin(), indicesTemplate.end(), [renderData](uint16_t& value) {
				value += renderData->getVertices().size();
				});

			renderData->addVertices(backVertices.begin(), backVertices.end(), indicesTemplate.begin(), indicesTemplate.end());
		}

		void addLeftFace(int32_t x, int32_t y, int32_t z, const blockFaceTextureFormat_t& textureFormat, graphics::ChunkRenderer::ChunkRenderData* renderData) {
			std::array<graphics::chunkVertex_t, 4> leftVertices = {
				graphics::chunkVertex_t{{0.0f + x, 1.0f + y, 0.0f + z}, textureFormat.topLeft}, // top-left back
				graphics::chunkVertex_t{{0.0f + x, 0.0f + y, 0.0f + z}, textureFormat.bottomLeft}, // bottom-left back
				graphics::chunkVertex_t{{0.0f + x, 0.0f + y, 1.0f + z}, textureFormat.bottomRight}, // bottom-left front
				graphics::chunkVertex_t{{0.0f + x, 1.0f + y, 1.0f + z}, textureFormat.topRight} // top-left front
			};

			std::array<uint16_t, 6> indicesTemplate = {
				0, 1, 3,
				1, 2, 3
			};
			std::for_each(indicesTemplate.begin(), indicesTemplate.end(), [renderData](uint16_t& value) {
				value += renderData->getVertices().size();
				});

			renderData->addVertices(leftVertices.begin(), leftVertices.end(), indicesTemplate.begin(), indicesTemplate.end());
		}

		void addRightFace(int32_t x, int32_t y, int32_t z, const blockFaceTextureFormat_t& textureFormat, graphics::ChunkRenderer::ChunkRenderData* renderData) {
			std::array<graphics::chunkVertex_t, 4> rightVertices = {
				graphics::chunkVertex_t{{1.0f + x, 1.0f + y, 0.0f + z}, textureFormat.topLeft}, // top-right back
				graphics::chunkVertex_t{{1.0f + x, 0.0f + y, 0.0f + z}, textureFormat.bottomLeft}, // bottom-right back
				graphics::chunkVertex_t{{1.0f + x, 0.0f + y, 1.0f + z}, textureFormat.bottomRight}, // bottom-right front
				graphics::chunkVertex_t{{1.0f + x, 1.0f + y, 1.0f + z}, textureFormat.topRight} // top-rigth front
			};

			std::array<uint16_t, 6> indicesTemplate = {
				0, 1, 3,
				1, 2, 3
			};
			std::for_each(indicesTemplate.begin(), indicesTemplate.end(), [renderData](uint16_t& value) {
				value += renderData->getVertices().size();
				});

			renderData->addVertices(rightVertices.begin(), rightVertices.end(), indicesTemplate.begin(), indicesTemplate.end());
		}

		void addTopFace(int32_t x, int32_t y, int32_t z, const blockFaceTextureFormat_t& textureFormat, graphics::ChunkRenderer::ChunkRenderData* renderData) {
			std::array<graphics::chunkVertex_t, 4> topVertices = {
				graphics::chunkVertex_t{{ 0.0f + x, 1.0f + y, 0.0f + z }, textureFormat.topLeft}, // top-left back
				graphics::chunkVertex_t{{ 1.0f + x, 1.0f + y, 0.0f + z }, textureFormat.bottomLeft}, // top-right back
				graphics::chunkVertex_t{{ 1.0f + x, 1.0f + y, 1.0f + z }, textureFormat.bottomRight}, // top-right front
				graphics::chunkVertex_t{{ 0.0f + x, 1.0f + y, 1.0f + z }, textureFormat.topRight} // top-left front
			};

			std::array<uint16_t, 6> indicesTemplate = {
				0, 1, 3,
				1, 2, 3
			};
			std::for_each(indicesTemplate.begin(), indicesTemplate.end(), [renderData](uint16_t& value) {
				value += renderData->getVertices().size();
				});
			renderData->addVertices(topVertices.begin(), topVertices.end(), indicesTemplate.begin(), indicesTemplate.end());
		}

		void addBottomFace(int32_t x, int32_t y, int32_t z, const blockFaceTextureFormat_t& textureFormat, graphics::ChunkRenderer::ChunkRenderData* renderData) {
			std::array<graphics::chunkVertex_t, 4> bottomVertices = {
				graphics::chunkVertex_t{{ 0.0f + x, 0.0f + y, 0.0f + z }, textureFormat.topLeft}, // bottom-left back
				graphics::chunkVertex_t{{ 1.0f + x, 0.0f + y, 0.0f + z }, textureFormat.bottomLeft}, // bottom-right back
				graphics::chunkVertex_t{{ 1.0f + x, 0.0f + y, 1.0f + z }, textureFormat.bottomRight}, // bottom-right front
				graphics::chunkVertex_t{{ 0.0f + x, 0.0f + y, 1.0f + z }, textureFormat.topRight} // bottom-left front
			};

			std::array<uint16_t, 6> indicesTemplate = {
				0, 1, 3,
				1, 2, 3
			};
			std::for_each(indicesTemplate.begin(), indicesTemplate.end(), [renderData](uint16_t& value) {
				value += renderData->getVertices().size();
				});
			renderData->addVertices(bottomVertices.begin(), bottomVertices.end(), indicesTemplate.begin(), indicesTemplate.end());
		}

		void pushQuad(int32_t x, int32_t y, int32_t z, uint16_t type1, QuadFace face, graphics::ChunkRenderer::ChunkRenderData* renderData) {
			std::array<graphics::chunkVertex_t, 4> leftRightVertices = {
				graphics::chunkVertex_t{{0.0f + x + (face == QuadFace::Right ? 1 : 0), 1.0f + y, 1.0f + z}, {0.0f, 1.0f}}, // top-left front
				graphics::chunkVertex_t{{0.0f + x + (face == QuadFace::Right ? 1 : 0), 0.0f + y, 1.0f + z}, {0.0f, 0.0f}}, // bottom-left front
				graphics::chunkVertex_t{{0.0f + x + (face == QuadFace::Right ? 1 : 0), 0.0f + y, 0.0f + z}, {0.2f, 0.0f}}, // bottom-left back
				graphics::chunkVertex_t{{0.0f + x + (face == QuadFace::Right ? 1 : 0), 1.0f + y, 0.0f + z}, {0.2f, 1.0f}} // top-left back
			};
			std::array<graphics::chunkVertex_t, 4> bottomTopVertices = {
				graphics::chunkVertex_t{{ 0.0f + x, 0.0f + y + (face == QuadFace::Top ? 1 : 0), 1.0f + z }, {0.0f, 0.0f}}, // bottom-left front
				graphics::chunkVertex_t{{ 1.0f + x, 0.0f + y + (face == QuadFace::Top ? 1 : 0), 1.0f + z }, {0.2f, 0.0f}}, // bottom-right front
				graphics::chunkVertex_t{{ 1.0f + x, 0.0f + y + (face == QuadFace::Top ? 1 : 0), 0.0f + z }, {0.2f, 1.0f}}, // bottom-right back
				graphics::chunkVertex_t{{ 0.0f + x, 0.0f + y + (face == QuadFace::Top ? 1 : 0), 0.0f + z }, {0.0f, 1.0f}} // bottom-left back
			};
			std::array<graphics::chunkVertex_t, 4> backFrontVertices = {
				graphics::chunkVertex_t{{0.0f + x, 1.0f + y, 0.0f + z + (face == QuadFace::Front ? 1 : 0)}, {0.0f, 1.0f}}, // top-left back
				graphics::chunkVertex_t{{0.0f + x, 0.0f + y, 0.0f + z + (face == QuadFace::Front ? 1 : 0)}, {0.0f, 0.0f}}, // bottom-left back
				graphics::chunkVertex_t{{1.0f + x, 0.0f + y, 0.0f + z + (face == QuadFace::Front ? 1 : 0)}, {0.2f, 0.0f}}, // bottomright back
				graphics::chunkVertex_t{{1.0f + x, 1.0f + y, 0.0f + z + (face == QuadFace::Front ? 1 : 0)}, {0.2f, 1.0f}} // top-right back
			};

			std::array<uint16_t, 6> indicesTemplate = {
				0, 1, 3,
				1, 2, 3
			};
			std::for_each(indicesTemplate.begin(), indicesTemplate.end(), [renderData](uint16_t& value) {
				value += renderData->getVertices().size();
			});

			if (face == QuadFace::Top || face == QuadFace::Bottom) {
				renderData->addVertices(bottomTopVertices.begin(), bottomTopVertices.end(), indicesTemplate.begin(), indicesTemplate.end());
			}
			else if (face == QuadFace::Left || face == QuadFace::Right) {
				renderData->addVertices(leftRightVertices.begin(), leftRightVertices.end(), indicesTemplate.begin(), indicesTemplate.end());
			}
			else if (face == QuadFace::Front || face == QuadFace::Back) {
				renderData->addVertices(backFrontVertices.begin(), backFrontVertices.end(), indicesTemplate.begin(), indicesTemplate.end());
			}
		}

		void createChunkMesh(const BlocksDatabase& blocksDatabase, chunk_t& chunk) {
			/* Simple culling algorithm */
			chunk.renderData->beginChunk();
			for (size_t x = 0; x < chunk.size; ++x) {
				for (size_t y = 0; y < chunk.size; ++y) {
					for (size_t z = 0; z < chunk.size; ++z) {
						if (chunk.blocks[get1DimIndex(x, y, z, chunk.size)] == 0) {
							continue;
						}
						//left
						if (x > 0 && !chunk.blocks[get1DimIndex(x - 1, y, z, chunk.size)]) {
							addLeftFace(x, y, z, blocksDatabase.getBlockTextureFormat(chunk.blocks[get1DimIndex(x, y, z, chunk.size)]).left, chunk.renderData);
						//	pushQuad(x, y, z, 1, QuadFace::Left, chunk.renderData);
						}
						//right
						if (x < chunk.size - 1 && !chunk.blocks[get1DimIndex(x + 1, y, z, chunk.size)]) {
							addRightFace(x, y, z, blocksDatabase.getBlockTextureFormat(chunk.blocks[get1DimIndex(x, y, z, chunk.size)]).right, chunk.renderData);
							//pushQuad(x, y, z, 1, QuadFace::Right, chunk.renderData);
						}
						//top
						if (y < chunk.size - 1 && !chunk.blocks[get1DimIndex(x, y + 1, z, chunk.size)]) {
							addTopFace(x, y, z, blocksDatabase.getBlockTextureFormat(chunk.blocks[get1DimIndex(x, y, z, chunk.size)]).top, chunk.renderData);
							//pushQuad(x, y, z, 1, QuadFace::Top, chunk.renderData);
						}
						// bottom
						if (y > 0 && !chunk.blocks[get1DimIndex(x, y - 1, z, chunk.size)]) {
							addBottomFace(x, y, z, blocksDatabase.getBlockTextureFormat(chunk.blocks[get1DimIndex(x, y, z, chunk.size)]).bottom, chunk.renderData);
							//pushQuad(x, y, z, 1, QuadFace::Bottom, chunk.renderData);
						}
						//front
						if (z < chunk.size - 1 && !chunk.blocks[get1DimIndex(x, y, z + 1, chunk.size)]) {
							addFrontFace(x, y, z, blocksDatabase.getBlockTextureFormat(chunk.blocks[get1DimIndex(x, y, z, chunk.size)]).front, chunk.renderData);
							//pushQuad(x, y, z, 1, QuadFace::Front, chunk.renderData);
						}
						//back
						if (z > 0 && !chunk.blocks[get1DimIndex(x, y, z - 1, chunk.size)]) {
							addBackFace(x, y, z, blocksDatabase.getBlockTextureFormat(chunk.blocks[get1DimIndex(x, y, z, chunk.size)]).back, chunk.renderData);
							//pushQuad(x, y, z, 1, QuadFace::Back, chunk.renderData);
						}
					}
				}
			}
		}
	}
}
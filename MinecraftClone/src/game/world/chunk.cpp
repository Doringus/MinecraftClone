#include "chunk.h"

#include <algorithm>
#include <array>

namespace game::world {

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

	void createChunkMesh(const BlocksDatabase& blocksDatabase, chunk_t& chunk) {
		/* Simple culling algorithm */
		chunk.renderData->beginChunk();
		for (size_t x = 0; x < chunk.box.width; ++x) {
			for (size_t y = 0; y < chunk.box.height; ++y) {
				for (size_t z = 0; z < chunk.box.depth; ++z) {
					if (chunk.blocks.get(x, y, z) == 0) {
						continue;
					}
					//left
					if (x > 0 && !chunk.blocks.get(x - 1, y, z)) {
						addLeftFace(x, y, z, blocksDatabase.getBlockTextureFormat(chunk.blocks.get(x, y, z)).left, chunk.renderData);
					}
					//right
					if (x < chunk.box.width - 1 && !chunk.blocks.get(x + 1, y, z)) {
						addRightFace(x, y, z, blocksDatabase.getBlockTextureFormat(chunk.blocks.get(x, y, z)).right, chunk.renderData);
					}
					//top
					if (y < chunk.box.height - 1 && !chunk.blocks.get(x, y + 1, z)) {
						addTopFace(x, y, z, blocksDatabase.getBlockTextureFormat(chunk.blocks.get(x, y, z)).top, chunk.renderData);
					}
					// bottom
					if (y > 0 && !chunk.blocks.get(x, y - 1, z)) {
						addBottomFace(x, y, z, blocksDatabase.getBlockTextureFormat(chunk.blocks.get(x, y, z)).bottom, chunk.renderData);
					}
					//front
					if (z < chunk.box.depth - 1 && !chunk.blocks.get(x, y, z + 1)) {
						addFrontFace(x, y, z, blocksDatabase.getBlockTextureFormat(chunk.blocks.get(x, y, z)).front, chunk.renderData);
					}
					//back
					if (z > 0 && !chunk.blocks.get(x, y, z - 1)) {
						addBackFace(x, y, z, blocksDatabase.getBlockTextureFormat(chunk.blocks.get(x, y, z)).back, chunk.renderData);
					}
				}
			}
		}
	}
}
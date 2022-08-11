#include "chunk.h"

#include <algorithm>
#include <array>

#include "../../renderer/igpubuffer.h"
#include "chunkshashmapstorage.h"

namespace game::world {

	void addIndices(std::vector<unsigned int>& indices, size_t verticesSize) {
		std::array<uint16_t, 6> indicesTemplate = {
				0, 1, 3,
				1, 2, 3
		};
		constexpr int verticesPerFace = 4;
		std::for_each(indicesTemplate.begin(), indicesTemplate.end(), [&indices, verticesSize, verticesPerFace](uint16_t& value) {
			indices.push_back(value + verticesSize - verticesPerFace);
		});
	}

	void addFrontFaceVertices(int32_t x, int32_t y, int32_t z, const blockFaceTextureFormat_t& textureFormat, std::vector<graphics::chunkVertex_t>& vertices) {
		vertices.push_back(graphics::chunkVertex_t{ {0.0f + x, 1.0f + y, 1.0f + z}, textureFormat.topLeft });
		vertices.push_back(graphics::chunkVertex_t{ {0.0f + x, 0.0f + y, 1.0f + z}, textureFormat.bottomLeft });
		vertices.push_back(graphics::chunkVertex_t{ {1.0f + x, 0.0f + y, 1.0f + z}, textureFormat.bottomRight });
		vertices.push_back(graphics::chunkVertex_t{ {1.0f + x, 1.0f + y, 1.0f + z}, textureFormat.topRight });
	}

	void addBackFaceVertices(int32_t x, int32_t y, int32_t z, const blockFaceTextureFormat_t& textureFormat, std::vector<graphics::chunkVertex_t>& vertices) {
		vertices.push_back(graphics::chunkVertex_t{ {0.0f + x, 1.0f + y, 0.0f + z}, textureFormat.topLeft }); // top-left back
		vertices.push_back(graphics::chunkVertex_t{ {0.0f + x, 0.0f + y, 0.0f + z}, textureFormat.bottomLeft }); // bottom-left back
		vertices.push_back(graphics::chunkVertex_t{ {1.0f + x, 0.0f + y, 0.0f + z}, textureFormat.bottomRight }); // bottom-right back
		vertices.push_back(graphics::chunkVertex_t{ {1.0f + x, 1.0f + y, 0.0f + z}, textureFormat.topRight }); // top-right back
	}

	void addLeftFaceVertices(int32_t x, int32_t y, int32_t z, const blockFaceTextureFormat_t& textureFormat, std::vector<graphics::chunkVertex_t>& vertices) {
		vertices.push_back(graphics::chunkVertex_t{ {0.0f + x, 1.0f + y, 0.0f + z}, textureFormat.topLeft }); // top-left back
		vertices.push_back(graphics::chunkVertex_t{ {0.0f + x, 0.0f + y, 0.0f + z}, textureFormat.bottomLeft }); // bottom-left back
		vertices.push_back(graphics::chunkVertex_t{ {0.0f + x, 0.0f + y, 1.0f + z}, textureFormat.bottomRight }); // bottom-left front
		vertices.push_back(graphics::chunkVertex_t{ {0.0f + x, 1.0f + y, 1.0f + z}, textureFormat.topRight }); // top-left front
	}

	void addRightFaceVertices(int32_t x, int32_t y, int32_t z, const blockFaceTextureFormat_t& textureFormat, std::vector<graphics::chunkVertex_t>& vertices) {
		vertices.push_back(graphics::chunkVertex_t{ {1.0f + x, 1.0f + y, 0.0f + z}, textureFormat.topLeft }); // top-right back
		vertices.push_back(graphics::chunkVertex_t{ {1.0f + x, 0.0f + y, 0.0f + z}, textureFormat.bottomLeft }); // bottom-right back
		vertices.push_back(graphics::chunkVertex_t{ {1.0f + x, 0.0f + y, 1.0f + z}, textureFormat.bottomRight }); // bottom-right front
		vertices.push_back(graphics::chunkVertex_t{ {1.0f + x, 1.0f + y, 1.0f + z}, textureFormat.topRight }); // top-rigth front
	}

	void addTopFaceVertices(int32_t x, int32_t y, int32_t z, const blockFaceTextureFormat_t& textureFormat, std::vector<graphics::chunkVertex_t>& vertices) {
		vertices.push_back(graphics::chunkVertex_t{ { 0.0f + x, 1.0f + y, 0.0f + z }, textureFormat.topLeft }); // top-left back
		vertices.push_back(graphics::chunkVertex_t{ { 1.0f + x, 1.0f + y, 0.0f + z }, textureFormat.bottomLeft }); // top-right back
		vertices.push_back(graphics::chunkVertex_t{ { 1.0f + x, 1.0f + y, 1.0f + z }, textureFormat.bottomRight }); // top-right front
		vertices.push_back(graphics::chunkVertex_t{ { 0.0f + x, 1.0f + y, 1.0f + z }, textureFormat.topRight }); // top-left front
	}

	void addBottomFaceVertices(int32_t x, int32_t y, int32_t z, const blockFaceTextureFormat_t& textureFormat, std::vector<graphics::chunkVertex_t>& vertices) {
		vertices.push_back(graphics::chunkVertex_t{ { 0.0f + x, 0.0f + y, 0.0f + z }, textureFormat.topLeft }); // bottom-left back
		vertices.push_back(graphics::chunkVertex_t{ { 1.0f + x, 0.0f + y, 0.0f + z }, textureFormat.bottomLeft }); // bottom-right back
		vertices.push_back(graphics::chunkVertex_t{ { 1.0f + x, 0.0f + y, 1.0f + z }, textureFormat.bottomRight }); // bottom-right front
		vertices.push_back(graphics::chunkVertex_t{ { 0.0f + x, 0.0f + y, 1.0f + z }, textureFormat.topRight }); // bottom-left front
	}

	void createChunkMesh(const ChunksHashmapStorage& storage, const BlocksDatabase& blocksDatabase, chunk_t& chunk) {
		/* Simple culling algorithm */
		std::vector<graphics::chunkVertex_t> vertices;
		std::vector<unsigned int> indices;
		for (size_t x = 0; x < chunk.box.width; ++x) {
			for (size_t y = 0; y < chunk.box.height; ++y) {
				for (size_t z = 0; z < chunk.box.depth; ++z) {
					if (y == 127 && x == 0 && chunk.box.xGrid == 0 && chunk.box.zGrid == 0) {
						int k = 0;
					}
					if (chunk.blocks.get(x, y, z) == BlockId::Air) {
						continue;
					}
					//left
					if (x > 0 && chunk.blocks.get(x - 1, y, z) == BlockId::Air) {
						addLeftFaceVertices(x, y, z, blocksDatabase.getBlockTextureFormat(chunk.blocks.get(x, y, z)).left, vertices);
						addIndices(indices, vertices.size());
					}
					//right
					if (x < chunk.box.width - 1 && chunk.blocks.get(x + 1, y, z) == BlockId::Air) {
						addRightFaceVertices(x, y, z, blocksDatabase.getBlockTextureFormat(chunk.blocks.get(x, y, z)).right, vertices);
						addIndices(indices, vertices.size());
					}
					//top
					if (y < chunk.box.height - 1 && chunk.blocks.get(x, y + 1, z) == BlockId::Air) {
						addTopFaceVertices(x, y, z, blocksDatabase.getBlockTextureFormat(chunk.blocks.get(x, y, z)).top, vertices);
						addIndices(indices, vertices.size());
					}
					// bottom
					if (y > 0 && chunk.blocks.get(x, y - 1, z) == BlockId::Air) {
						addBottomFaceVertices(x, y, z, blocksDatabase.getBlockTextureFormat(chunk.blocks.get(x, y, z)).bottom, vertices);
						addIndices(indices, vertices.size());
					}
					//front
					if (z < chunk.box.depth - 1 && chunk.blocks.get(x, y, z + 1) == BlockId::Air) {
						addFrontFaceVertices(x, y, z, blocksDatabase.getBlockTextureFormat(chunk.blocks.get(x, y, z)).front, vertices);
						addIndices(indices, vertices.size());
					}
					//back
					if (z > 0 && chunk.blocks.get(x, y, z - 1) == BlockId::Air) {
						addBackFaceVertices(x, y, z, blocksDatabase.getBlockTextureFormat(chunk.blocks.get(x, y, z)).back, vertices);
						addIndices(indices, vertices.size());
					}
				}
			}
		}

		// fill border blocks

		//Right border
		if (auto borderChunk = storage.getChunk(chunk.box.xGrid + 1, chunk.box.zGrid); borderChunk) {
			for (int z = 0; z < chunk.box.depth; ++z) {
				for (int y = 0; y < chunk.box.height; ++y) {
					// for current chunk
					if ((*borderChunk)->blocks.get(0, y, z) == BlockId::Air && chunk.blocks.get(chunk.box.width - 1, y, z) != BlockId::Air) {
						addRightFaceVertices(chunk.box.width - 1, y, z, blocksDatabase.getBlockTextureFormat(chunk.blocks.get(chunk.box.width - 1, y, z)).right, vertices);
						addIndices(indices, vertices.size());
					}
				}
			}
		}
		//Left border
		if (auto borderChunk = storage.getChunk(chunk.box.xGrid - 1, chunk.box.zGrid); borderChunk) {
			for (int z = 0; z < chunk.box.depth; ++z) {
				for (int y = 0; y < chunk.box.height; ++y) {
					// for current chunk
					if ((*borderChunk)->blocks.get(chunk.box.width - 1, y, z) == BlockId::Air && chunk.blocks.get(0, y, z) != BlockId::Air) {
						addLeftFaceVertices(0, y, z, blocksDatabase.getBlockTextureFormat(chunk.blocks.get(0, y, z)).left, vertices);
						addIndices(indices, vertices.size());
					}
				}
			}
		}

		// front border
		if (auto borderChunk = storage.getChunk(chunk.box.xGrid, chunk.box.zGrid + 1); borderChunk) {
			for (int x = 0; x < chunk.box.width; ++x) {
				for (int y = 0; y < chunk.box.height; ++y) {
					// for current chunk
					if ((*borderChunk)->blocks.get(x, y, 0) == BlockId::Air && chunk.blocks.get(x, y, chunk.box.depth - 1) != BlockId::Air) {
						addFrontFaceVertices(x, y, chunk.box.depth - 1, blocksDatabase.getBlockTextureFormat(chunk.blocks.get(x, y, chunk.box.depth - 1)).front, vertices);
						addIndices(indices, vertices.size());
					}
				}
			}
		}
		// back border
		if (auto borderChunk = storage.getChunk(chunk.box.xGrid, chunk.box.zGrid - 1); borderChunk) {
			for (int x = 0; x < chunk.box.width; ++x) {
				for (int y = 0; y < chunk.box.height; ++y) {
					// for current chunk
					if ((*borderChunk)->blocks.get(x, y, chunk.box.depth - 1) == BlockId::Air && chunk.blocks.get(x, y, 0) != BlockId::Air) {
						addBackFaceVertices(x, y, 0, blocksDatabase.getBlockTextureFormat(chunk.blocks.get(x, y, 0)).back, vertices);
						addIndices(indices, vertices.size());
					}
				}
			}
		}
		chunk.renderData->getVertexBuffer()->appendData(vertices.size() * sizeof(graphics::chunkVertex_t), vertices.data());
		chunk.renderData->getIndexBuffer()->appendData(indices.size() * sizeof(unsigned int), indices.data());
	}
}
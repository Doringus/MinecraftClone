#pragma once

#include "ichunksstorage.h"

#include <unordered_map>

namespace game::world {

	class ChunksHashmapStorage : public IChunksStorage {
	public:
		std::optional<chunk_t*> getChunk(int64_t x, int64_t z) const override;
		bool contains(int64_t x, int64_t z) const noexcept override;
		std::optional<std::unique_ptr<chunk_t>> takeChunk(int64_t x, int64_t z) override;
		void insertChunk(std::unique_ptr<chunk_t> chunk) override;
		void removeChunk(int64_t x, int64_t z) override;
	private:
		struct chunkHash_t {
			template <class T1, class T2>
			std::size_t operator() (const std::pair<T1, T2>& pair) const {
				return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
			}
		};
	private:
		std::unordered_map<std::pair<int64_t, int64_t>, std::unique_ptr<chunk_t>, chunkHash_t> m_Chunks;
	};

}
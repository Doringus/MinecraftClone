#pragma once


#include <unordered_map>
#include <optional>

#include "chunk.h"

namespace game::world {

	struct chunkHash_t {
		template <class T1, class T2>
		std::size_t operator() (const std::pair<T1, T2>& pair) const {
			return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
		}
	};

	class ChunksHashmapStorage {
	public:
		std::optional<chunk_t*> getChunk(int64_t x, int64_t z) const;
		bool contains(int64_t x, int64_t z) const noexcept;
		std::optional<std::unique_ptr<chunk_t>> takeChunk(int64_t x, int64_t z);
		void insertChunk(std::unique_ptr<chunk_t> chunk);
		void removeChunk(int64_t x, int64_t z);

		auto erase(auto it) {
			return m_Chunks.erase(it);
		}

		auto begin() {
			return m_Chunks.begin();
		}

		auto end() {
			return m_Chunks.end();
		}
		
	private:
		std::unordered_map<std::pair<int64_t, int64_t>, std::unique_ptr<chunk_t>, chunkHash_t> m_Chunks;
	};

}
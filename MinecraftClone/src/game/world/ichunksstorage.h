#pragma once



namespace game::world {

	class IChunksStorage {
	public:
		virtual ~IChunksStorage() = default;

		virtual std::optional<chunk_t*> getChunk(int64_t x, int64_t z) const = 0;
		virtual bool contains(int64_t x, int64_t z) const noexcept = 0;
		virtual std::optional<std::unique_ptr<chunk_t>> takeChunk(int64_t x, int64_t z) = 0;
		virtual void insertChunk(std::unique_ptr<chunk_t> chunk) = 0;
		virtual void removeChunk(int64_t x, int64_t z) = 0;
	};

}
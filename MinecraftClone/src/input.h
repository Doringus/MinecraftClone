#pragma once

#include <utility>
#include <memory>
#include <array>

enum class GameKey {
	KEY_W = 0,
	KEY_A,
	KEY_S,
	KEY_D
};

constexpr std::size_t KEYS_COUNT = 4;

class IKeyWrapper {
public:
	virtual ~IKeyWrapper() = default;

	virtual int32_t getAPIKey(GameKey key) const noexcept = 0;
	virtual int32_t getGameKey(int32_t apiKey) const noexcept = 0;
};

class Input {
public:
	virtual ~Input() = default;

	virtual void update() noexcept = 0;
	virtual bool isKeyPressed(GameKey gameKey) const noexcept = 0;
	virtual std::pair<double, double> getMousePosition() const noexcept = 0;
protected:
	std::unique_ptr<IKeyWrapper> m_Wrapper;
	std::array<bool, KEYS_COUNT> m_Keys;
};

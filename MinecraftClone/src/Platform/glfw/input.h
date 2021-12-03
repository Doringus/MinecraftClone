#pragma once

#include <array>
#include <memory>

#include "../../iwindow.h"

constexpr std::size_t KEYS_COUNT = 4;

enum class GameKey {
	KEY_W = 0,
	KEY_A,
	KEY_S,
	KEY_D
};

class Input {

	class InputMapper {
	public:
		InputMapper() = default;
		~InputMapper() = default;

		int32_t getApiKey(GameKey gameKey) const noexcept;
	};

public:
	Input(std::shared_ptr<IWindow> window);
	~Input() = default;

	void update() noexcept;
	bool isKeyPressed(GameKey key) const noexcept;

private:
	std::shared_ptr<IWindow> m_Window;
	InputMapper m_Mapper;
	std::array<bool, KEYS_COUNT> m_Keys;
};
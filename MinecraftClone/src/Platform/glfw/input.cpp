#include "input.h"

#include <GLFW/glfw3.h>

Input::Input(std::shared_ptr<IWindow> window) : m_Window(std::move(window)) {

}

void Input::update() noexcept {
	for (std::size_t i = 0; i < KEYS_COUNT; ++i) {
		m_Keys[i] = glfwGetKey(static_cast<GLFWwindow*>(m_Window->getNativeWindow()), 
			m_Mapper.getApiKey(static_cast<GameKey>(i)));
	}
}

bool Input::isKeyPressed(GameKey key) const noexcept {
	return m_Keys[static_cast<std::size_t>(key)];
}

int32_t Input::InputMapper::getApiKey(GameKey gameKey) const noexcept {
	switch (gameKey) {
		case GameKey::KEY_W: {
			return GLFW_KEY_W;
		}
		case GameKey::KEY_A: {
			return GLFW_KEY_A;
		}
		case GameKey::KEY_S: {
			return GLFW_KEY_S;
		}
		case GameKey::KEY_D: {
			return GLFW_KEY_D;
		}
	}
}

#include "glfwinput.h"

int32_t GlfwKeyWrapper::getAPIKey(GameKey key) const noexcept {
	switch (key) {
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

int32_t GlfwKeyWrapper::getGameKey(int32_t apiKey) const noexcept {
	return int32_t();
}


GlfwInput::GlfwInput(GLFWwindow * const window, std::unique_ptr<IKeyWrapper> wrapper) : m_Window(window) {
	m_Wrapper = std::make_unique<GlfwKeyWrapper>();
}

void GlfwInput::update() noexcept {
	for (std::size_t i = 0; i < KEYS_COUNT; ++i) {
		m_Keys[i] = glfwGetKey(m_Window, m_Wrapper->getAPIKey(static_cast<GameKey>(i)));
	}
}

bool GlfwInput::isKeyPressed(GameKey gameKey) const noexcept {
	return m_Keys[static_cast<int>(GameKey::KEY_A)];
}

std::pair<double, double> GlfwInput::getMousePosition() const noexcept {
	double x, y;
	glfwGetCursorPos(m_Window, &x, &y);
	return std::pair<double, double>(x, y);
}
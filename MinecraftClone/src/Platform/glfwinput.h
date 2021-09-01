#pragma once

#include <GLFW/glfw3.h>

#include "../input.h"

class GlfwKeyWrapper : public IKeyWrapper {
	virtual int32_t getAPIKey(GameKey key) const noexcept;
	virtual int32_t getGameKey(int32_t apiKey) const noexcept;
};

class GlfwInput : public Input {
public:
	GlfwInput(GLFWwindow* const window, std::unique_ptr<IKeyWrapper> wrapper);
	
	void update() noexcept override;
	bool isKeyPressed(GameKey gameKey) const noexcept override;
	std::pair<double, double> getMousePosition() const noexcept override;
private:
	GLFWwindow* const m_Window;
};
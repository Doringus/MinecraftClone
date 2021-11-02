#pragma once

#include "../../../Renderer/renderer.h"

struct GLFWwindow;
class IWindow;

class OpenGLRenderer : public IRenderer {
public:
	OpenGLRenderer(IWindow* window) noexcept;

	void beginFrame() noexcept override;
	void push(batch_t& batch) noexcept override;
	void draw() const noexcept override;
	void endFrame() noexcept override;

private:
	IWindow* m_Window;
};
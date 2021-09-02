#pragma once

#include "../../renderingcontext.h"

struct GLFWwindow;

class OpenGLRenderingContext : public IRenderingContext {
public:
	OpenGLRenderingContext(GLFWwindow* window) noexcept;

	void init() const override;
	void swapBuffers() const override;
private:
	GLFWwindow* m_Window;
};
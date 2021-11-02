#pragma once

class IWindow;
class Input;
class IRenderer;

class Application {
public:
	Application() noexcept;
	~Application();

	void run();
private:
	bool m_IsRunning;
	IWindow* m_Window;
	Input* m_Input;
	IRenderer* m_Renderer;
};
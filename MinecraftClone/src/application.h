#pragma once

class Window;
class Input;

class Application {
public:
	Application() noexcept;

	void run();
private:
	bool m_IsRunning;
	Window* m_Window;
	Input* m_Input;
};
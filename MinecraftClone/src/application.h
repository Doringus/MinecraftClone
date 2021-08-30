#pragma once

class Window;

class Application {
public:
	Application() noexcept;

	void run();
private:
	bool m_IsRunning;
	Window* m_Window;
};
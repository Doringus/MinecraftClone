#pragma once

#include "iwindow.h"

#include <memory>

class Input;

class Application {
public:
	Application() noexcept;
	virtual ~Application() = default;

	Application(const Application&) = delete;
	Application(Application&&) = delete;
	Application& operator=(const Application&) = delete;
	Application& operator=(Application&&) = delete;

	void run();
private:
	bool m_IsRunning;
	Input* m_Input;
	std::shared_ptr<IWindow> m_Window;
};
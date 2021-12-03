#pragma once

#include <string>
#include <cinttypes>


class IWindow {
public:
	IWindow(const std::string& title, int32_t width, int32_t height) :
		m_Title(title), m_Width(width), m_Height(height), 
		m_FramebufferWidth(width), m_FramebufferHeight(height) {}
	virtual ~IWindow() = default;

	virtual void* getNativeWindow() const noexcept = 0;
	virtual void pollEvents() const noexcept = 0;
	virtual bool isOpen() const noexcept = 0;
	virtual void swapBuffers() const noexcept = 0;
protected:
	int32_t m_Width, m_Height;
	int32_t m_FramebufferWidth, m_FramebufferHeight;
	std::string m_Title;
};
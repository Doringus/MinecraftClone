#pragma once

#include <cstdint>
#include <string>

#include "Renderer/renderer.h"

class IWindow {
public:

	IWindow(uint32_t width = 640, uint32_t height = 480, const std::string& title = "window", RendererApi rendererApi = RendererApi::OpenGL) noexcept : 
		m_Width(width), m_Height(height), m_Title(title) {}
	
	virtual ~IWindow() = default;

	virtual bool isOpen() const noexcept = 0;
	virtual void pollEvents() const = 0;
	virtual void* getNativeWindow() const noexcept = 0;

	uint32_t getWidth() const noexcept {
		return m_Width;
	}

	uint32_t getHeight() const noexcept {
		return m_Height;
	}

	uint32_t getFramebufferWidth() const noexcept {
		return m_FramebufferWidth;
	}

	uint32_t getFramebufferHeight() const noexcept {
		return m_FramebufferHeight;
	}

	std::string getTitle() const noexcept {
		return m_Title;
	}
	
	virtual void resizeWindow(int32_t widht, int32_t height) = 0;

protected:
	void setFramebufferWidth(uint32_t width) {
		m_FramebufferWidth = width;
	}

	void setFramebufferHeight(uint32_t height) {
		m_FramebufferHeight = height;
	}

	void setWidth(uint32_t width) noexcept {
		m_Width = width;
	}

	void setHeight(uint32_t height) noexcept {
		m_Height = height;
	}
protected:
	uint32_t m_Width, m_Height;
	uint32_t m_FramebufferWidth, m_FramebufferHeight;

	std::string m_Title;
};
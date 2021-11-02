#pragma once

#include <vector>

enum class RendererApi {
	OpenGL = 0
};

enum class DepthFunction {
	NEVER = 0,
	LESS,
	EQUAL,
	LEQUAL,
	GREATER, 
	NOTEQUAL, 
	GEQUAL, 
	ALWAYS,
};

struct renderState_t {
	DepthFunction function;
	bool enableDepthTest;
};

class IBuffer;
class ITexture;
class IShader;

struct batch_t {
	renderState_t state;
	std::vector<IBuffer*> vertexBuffers;
	std::vector<IBuffer*> indexBuffers;	
	ITexture* texture;
	IShader* shader;
};

class IRenderer {
public:
	IRenderer(uint32_t width, uint32_t height) noexcept : m_Width(width), m_Height(height) {}
	virtual ~IRenderer() = default;

	virtual void beginFrame() noexcept = 0;
	virtual void push(batch_t& batch) noexcept = 0;
	virtual void draw() const noexcept = 0;
	virtual void endFrame() noexcept = 0;
protected:
	uint32_t m_Width, m_Height;
};

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
	IRenderer(uint32_t width, uint32_t height) noexcept : m_ViewportWidth(width), m_ViewportHeight(height), m_RenderState({DepthFunction::LESS, true}) {}
	
	virtual bool init() noexcept = 0;
	virtual void clearBuffers() const noexcept = 0;
	virtual void drawIndexed(batch_t* batch) noexcept = 0;
	virtual void drawElements(batch_t* batch) noexcept = 0;
protected:
	virtual void setRenderState(const renderState_t& renderState) noexcept = 0;
protected:
	uint32_t m_ViewportWidth, m_ViewportHeight;
	renderState_t m_RenderState;
};
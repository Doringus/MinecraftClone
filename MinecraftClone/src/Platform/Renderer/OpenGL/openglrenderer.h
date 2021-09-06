#pragma once

#include "../../../Renderer/renderer.h"


class OpenGLRenderer : public IRenderer {
public:
	OpenGLRenderer(uint32_t width, uint32_t height) noexcept;
	
	bool init() noexcept override;
	void clearBuffers() const noexcept override;
	void drawIndexed(batch_t* batch) noexcept override;
	void drawElements(batch_t* batch) noexcept override;
protected:
	void setRenderState(const renderState_t& renderState) noexcept override;
};
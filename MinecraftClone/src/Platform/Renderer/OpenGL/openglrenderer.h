#pragma once

#include "../../../renderer/renderer.h"

class OpenglRenderer : public IRenderer {
public:
	
	void beginFrame() override;
	void submit() override;
	void render() override;
};
#pragma once

#include "../../../renderer/irenderercontext.h"

namespace graphics::opengl {

	class OpenglRendererContext : public IRendererContext {
	public:
		OpenglRendererContext();

		void clearScreen(float r, float g, float b, float alpha);
		void clearFramebuffer();
	};

}
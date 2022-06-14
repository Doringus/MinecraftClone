#pragma once

namespace graphics {

	class IRendererContext {
	public:
		virtual ~IRendererContext() = default;

		virtual void clearScreen(float r, float g, float b, float alpha) = 0;
		virtual void clearFramebuffer() = 0;
	};

}
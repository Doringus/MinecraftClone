#pragma once


#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../../../../renderer/igpubuffer.h"

namespace graphics::opengl {

	class OpenglInputLayout;

	class OpenglVertexBuffer : public IGpuBuffer {
	public:
		OpenglVertexBuffer(size_t elementSize);
		~OpenglVertexBuffer();

		size_t elementsCount() const noexcept;
		void* map() override;
		void release() override;
		void write(size_t size, void* data) override;

		void bind(GLuint vaoHandle) const;
	private:
		size_t m_ElementSize, m_SizeInBytes = 0;
		GLuint m_Handle = 0;
	};

}
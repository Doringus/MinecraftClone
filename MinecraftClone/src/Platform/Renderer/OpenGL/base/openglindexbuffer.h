#pragma once


#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../../../../renderer/igpubuffer.h"

namespace graphics::opengl {

	class OpenglInputLayout;

	class OpenglIndexBuffer : public IGpuBuffer {
	public:
		OpenglIndexBuffer();
		~OpenglIndexBuffer();

		size_t elementsCount() const noexcept;
		void* map() override;
		void release() override;
		void write(size_t size, void* data) override;

		void bind(GLuint vaoHandle) const;
	private:
		size_t m_ElementSize = sizeof(unsigned int), m_SizeInBytes = 0;
		GLuint m_Handle;
	};

}
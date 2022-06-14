#pragma once


#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace graphics {
	namespace opengl {

		class OpenglInputLayout;

		class OpenglIndexBuffer {
		public:
			OpenglIndexBuffer(OpenglInputLayout const* layout);
			~OpenglIndexBuffer();

			void setBuffer(void const* data, size_t totalSize, size_t elementSize);
			void bind() const;
		private:
			size_t m_ElementSize = 0;
			GLuint m_Handle;
			OpenglInputLayout const* m_Layout;
		};

	}

}
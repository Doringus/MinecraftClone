#pragma once


#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>



namespace graphics {
	namespace opengl {

		class OpenglInputLayout;

		class OpenglVertexBuffer {
		public:
			OpenglVertexBuffer(OpenglInputLayout const* layout);
			~OpenglVertexBuffer();

			void setBuffer(void const* data, size_t totalSize, size_t elementSize);
			void* mapBuffer();
			void unmapBuffer();
			void bind() const;
		private:
			size_t m_ElementSize;
			GLuint m_Handle;
			OpenglInputLayout const* m_Layout;
		};

	}
}
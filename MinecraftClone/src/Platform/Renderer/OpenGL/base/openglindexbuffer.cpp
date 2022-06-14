#include "openglindexbuffer.h"

#include "openglinputlayout.h"

namespace graphics {
	namespace opengl {
		
		OpenglIndexBuffer::OpenglIndexBuffer(OpenglInputLayout const* layout) : m_Layout(layout) {
			glCreateBuffers(1, &m_Handle);
		}

		OpenglIndexBuffer::~OpenglIndexBuffer() {
			glDeleteBuffers(1, &m_Handle);
		}

		void OpenglIndexBuffer::setBuffer(void const* data, size_t totalSize, size_t elementSize) {
			m_ElementSize = elementSize;
			glNamedBufferData(m_Handle, totalSize, data, GL_DYNAMIC_DRAW);
		}
	
		void OpenglIndexBuffer::bind() const {
			glVertexArrayElementBuffer(m_Layout->getId(), m_Handle);
		}
	}

}
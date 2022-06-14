#include "openglinputlayout.h"


using namespace graphics::opengl;

OpenglInputLayout::OpenglInputLayout(const BufferLayout& layout) : m_Layout(layout) {
	glCreateVertexArrays(1, &m_Id);

	int offset = 0;
	for (int i = 0; const auto & element : m_Layout.getElementsRef()) {
		glVertexArrayAttribBinding(m_Id, i, 1);
		glVertexArrayAttribFormat(m_Id, i, element.attributesCount, getShaderDataType(element.type), GL_FALSE, offset);
		glEnableVertexArrayAttrib(m_Id, i);
		++i;
		offset += element.size;
	}
}

OpenglInputLayout::~OpenglInputLayout() {
	glDeleteVertexArrays(1, &m_Id);

}

void OpenglInputLayout::bind() const {
	glBindVertexArray(m_Id);
}

void OpenglInputLayout::unbind() const {
	glBindVertexArray(0);
}

GLuint OpenglInputLayout::getId() const noexcept {
	return m_Id;
}

#pragma once

#include "../../../renderer/iinputlayout.h"

#include <GL/glew.h>

#include "../../../renderer/bufferlayout.h"

class OpenglInputLayout : public IInputLayout {
public:
	explicit OpenglInputLayout(const BufferLayout& layout);
	~OpenglInputLayout();

	void bind() override;
	void unbind() override;

	GLuint getId() const noexcept;

private:
	BufferLayout m_Layout;
	GLuint m_Id;
};
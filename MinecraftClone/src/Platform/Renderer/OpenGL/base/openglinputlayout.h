#pragma once

#include <GL/glew.h>

#include "../../../../renderer/bufferlayout.h"

namespace graphics {
	namespace opengl {

		class OpenglInputLayout {
		public:
			explicit OpenglInputLayout(const BufferLayout& layout);
			~OpenglInputLayout();

			void bind() const;
			void unbind() const;

			GLuint getId() const noexcept;

		private:
			BufferLayout m_Layout;
			GLuint m_Id;
		};

	}
}
#pragma once


#include <vector>
#include <filesystem>
#include <unordered_map>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../../../../vendor/glm/mat4x4.hpp"

namespace graphics {
	namespace opengl {

		class OpenglShader {
		public:
			explicit OpenglShader(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);
			OpenglShader(const std::filesystem::path& vertexShaderPath, const std::filesystem::path& fragmentShaderPath);
			~OpenglShader() noexcept;

			void use() const;
			void setUniformMat4(const std::string& name, const glm::mat4& matrix);
			void setUniform1i(const std::string& name, int64_t value);
		private:
			std::unordered_map<std::string, GLint> m_UniformLocations;
			GLuint m_ProgramId;
		};

	}
}
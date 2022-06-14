#pragma once

#include "../../vendor/glm/vec3.hpp"
#include "../../vendor/glm/matrix.hpp"


class Input;

namespace graphics {

	class Camera {
	public:
		Camera() = default;
		Camera(const glm::mat4& projection, const glm::vec3& position = glm::vec3(0.f, 0.f, 0.f),
			const glm::vec3& front = glm::vec3(0.f, 0.f, -1.f), const glm::vec3& up = glm::vec3(0.f, 1.f, 0.f)) noexcept;

		// returns projection * view matrix
		glm::mat4 calculateCameraMatrix() const noexcept;
		glm::vec3 position() const noexcept;

		void update(const Input& input, float dt);

	private:
		double m_Pitch, m_Yaw;
		glm::vec3 m_Position;
		glm::vec3 m_Front;
		glm::vec3 m_Up;
		glm::mat4 m_ProjectionMatrix;
	};

}
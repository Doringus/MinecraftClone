#include "camera.h"

#include "../../vendor/glm/gtc/matrix_transform.hpp"

namespace graphics {

	Camera::Camera(const glm::mat4& projection, const glm::vec3& position, const glm::vec3& front, const glm::vec3& up) noexcept
		: m_ProjectionMatrix(projection), m_Position(position), m_Front(front), m_Up(up), m_Pitch(0.f), m_Yaw(-90.f) { }

	glm::mat4 Camera::calculateCameraMatrix() const noexcept {
		return m_ProjectionMatrix * glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	}

	glm::vec3 Camera::position() const noexcept {
		return m_Position;
	}

}
#include "camera.h"

#include "../../vendor/glm/gtc/matrix_transform.hpp"

#include "../platform/glfw/input.h"

#include <iostream>

namespace graphics {

	Camera::Camera(const glm::mat4& projection, const glm::vec3& position, const glm::vec3& front, const glm::vec3& up) noexcept
		: m_ProjectionMatrix(projection), m_Position(position), m_Front(front), m_Up(up), m_Pitch(0.f), m_Yaw(-90.f) {
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	}

	glm::mat4 Camera::calculateCameraMatrix() const noexcept {
		return m_ProjectionMatrix * m_ViewMatrix;
	}

	glm::mat4 Camera::getViewMatrix() const noexcept {
		return m_ViewMatrix;
	}

	glm::mat4 Camera::getProjectionMatrix() const noexcept {
		return m_ProjectionMatrix;
	}

	glm::vec3 Camera::position() const noexcept {
		return m_Position;
	}

	void Camera::update(const Input& input, float dt) {
		/* Move camera */
		if (input.isKeyPressed(GameKey::KEY_W)) {
			m_Position +=  (3.f * dt) * m_Front;
		}
		if (input.isKeyPressed(GameKey::KEY_S)) {
			m_Position -= (3.f * dt) * m_Front;
		}
		if (input.isKeyPressed(GameKey::KEY_A)) {
			m_Position -= glm::normalize(glm::cross(m_Front, m_Up)) * (3.f * dt);
		}
		if (input.isKeyPressed(GameKey::KEY_D)) {
			m_Position += glm::normalize(glm::cross(m_Front, m_Up)) * (3.f * dt);
		}
		/* Rotate camera */
		auto currentPosition = input.getCurrentMousePosition();
		auto prevPosition = input.getPreviousMousePosition();
		double offsetX = currentPosition.x - prevPosition.x;
		double offsetY = currentPosition.y - prevPosition.y;
		offsetX *= 3.f * dt;
		offsetY *= 3.f * dt;
		m_Yaw += offsetX;
		m_Pitch -= offsetY;
		if (m_Pitch > 89.0f) {
			m_Pitch = 89.0f;
		}
		if (m_Pitch < -89.0f) {
			m_Pitch = -89.0f;
		}
		glm::vec3 front = glm::vec3(cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)), 
									sin(glm::radians(m_Pitch)),
									sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)));
		m_Front = glm::normalize(front);
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	}

}
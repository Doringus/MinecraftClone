#include "camera.h"

#include "../../vendor/glm/gtc/matrix_transform.hpp"

#include "../platform/glfw/input.h"
#include "../game/transformcomponent.h"

#include <iostream>

namespace graphics {

	Camera::Camera(const glm::mat4& projection) noexcept
		: m_ProjectionMatrix(projection) { }

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
		return m_TransformComponent->position;
	}

	void Camera::attachToObject(game::transformComponent_t* transformComponent) noexcept {
		m_TransformComponent = transformComponent;
	}

	void Camera::update() {
		m_ViewMatrix = glm::lookAt(m_TransformComponent->position, m_TransformComponent->position + m_TransformComponent->front, m_TransformComponent->up);
	}

}
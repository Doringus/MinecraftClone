#include "player.h"

#include "../renderer/camera.h"
#include "../platform/glfw/input.h"
#include "world/chunksmanager.h"


#include "../../vendor/glm/gtc/matrix_transform.hpp"
#include "../../vendor/glm/glm.hpp"
#include "../../vendor/glm/gtc/type_ptr.hpp"

namespace game {

	Player::Player(const glm::vec3& position, graphics::Camera* camera) noexcept {
		camera->attachToObject(&m_TransformComponent);
		m_TransformComponent = { position, glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f), 0, 0 };
	}

	void Player::update(const Input& input, float dt) {
		if (input.isKeyPressed(GameKey::KEY_W)) {
			m_TransformComponent.position += (3.f * dt) * m_TransformComponent.front;
		}
		if (input.isKeyPressed(GameKey::KEY_S)) {
			m_TransformComponent.position -= (3.f * dt) * m_TransformComponent.front;
		}
		if (input.isKeyPressed(GameKey::KEY_A)) {
			m_TransformComponent.position -= glm::normalize(glm::cross(m_TransformComponent.front, m_TransformComponent.up)) * (3.f * dt);
		}
		if (input.isKeyPressed(GameKey::KEY_D)) {
			m_TransformComponent.position += glm::normalize(glm::cross(m_TransformComponent.front, m_TransformComponent.up)) * (3.f * dt);
		}
		/* Rotate camera */
		auto currentPosition = input.getCurrentMousePosition();
		auto prevPosition = input.getPreviousMousePosition();
		double offsetX = currentPosition.x - prevPosition.x;
		double offsetY = currentPosition.y - prevPosition.y;
		offsetX *= 3.f * dt;
		offsetY *= 3.f * dt;
		m_TransformComponent.yaw += offsetX;
		m_TransformComponent.pitch -= offsetY;
		if (m_TransformComponent.pitch > 89.0f) {
			m_TransformComponent.pitch = 89.0f;
		}
		if (m_TransformComponent.pitch < -89.0f) {
			m_TransformComponent.pitch = -89.0f;
		}
		glm::vec3 front = glm::vec3(cos(glm::radians(m_TransformComponent.yaw)) * cos(glm::radians(m_TransformComponent.pitch)),
			sin(glm::radians(m_TransformComponent.pitch)),
			sin(glm::radians(m_TransformComponent.yaw)) * cos(glm::radians(m_TransformComponent.pitch)));
		m_TransformComponent.front = glm::normalize(front);
	}

	void Player::checkCollisionWithWorld(const world::ChunksManager& chunksManager) {

	}

}
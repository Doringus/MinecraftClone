#pragma once

#include "../../vendor/glm/vec3.hpp"
#include "../../vendor/glm/matrix.hpp"

class Input;

namespace game {
	struct transformComponent_t;
}

namespace graphics {

	class Camera {
	public:
		Camera() = default;
		Camera(const glm::mat4& projection) noexcept;

		// returns projection * view matrix
		glm::mat4 calculateCameraMatrix() const noexcept;
		glm::mat4 getViewMatrix() const noexcept;
		glm::mat4 getProjectionMatrix() const noexcept;
		glm::vec3 position() const noexcept;

		void attachToObject(game::transformComponent_t* transformComponent) noexcept;
		
		void update();

	private:
		game::transformComponent_t* m_TransformComponent{nullptr};
		glm::mat4 m_ProjectionMatrix, m_ViewMatrix;
	};

}
#pragma once

#include "../../vendor/glm/vec3.hpp"

namespace game {

	struct transformComponent_t {
		glm::vec3 position;
		glm::vec3 front;
		glm::vec3 up;
		double yaw, pitch;
	};

}
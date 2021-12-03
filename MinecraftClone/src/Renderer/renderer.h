#pragma once

#include "../../vendor/glm/vec2.hpp"
#include "../../vendor/glm/vec3.hpp"


struct vertex_t {
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texCoords;
};

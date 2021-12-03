#pragma once

#include "../../vendor/glm/vec2.hpp"
#include "../../vendor/glm/vec3.hpp"


struct vertex_t {
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texCoords;
};

class IRenderer {
public:
	virtual ~IRenderer() = default;

	virtual void beginFrame() = 0;
	virtual void submit() = 0;
	virtual void render() = 0;
};
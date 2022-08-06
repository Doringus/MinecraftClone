#pragma once

#include "transformcomponent.h"

namespace graphics {
	class Camera;
}

namespace game::world {
	class ChunksManager;
}

class Input;

namespace game {

	class world::ChunksManager;

	class Player {
	public:

		Player(const glm::vec3& position, graphics::Camera* camera) noexcept;

		void update(const Input& input, float dt);
		void checkCollisionWithWorld(const world::ChunksManager& chunksManager);

	private:
		transformComponent_t m_TransformComponent;
	};

}
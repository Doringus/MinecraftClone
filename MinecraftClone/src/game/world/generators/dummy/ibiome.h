#pragma once

#include <string>

namespace game::world {

	class IBiome {
	public:
		virtual std::string getName() const noexcept = 0;
	};

}
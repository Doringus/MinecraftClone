#pragma once

#include <string>
#include <filesystem>

namespace utils {

	std::string readEntireFileToStr(const std::filesystem::path& path);

}
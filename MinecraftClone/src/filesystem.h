#pragma once

#include <filesystem>

class Filesystem {
public:
	void setRoot(const std::filesystem::path& path) noexcept;
	std::string readFile(const std::filesystem::path& path);
private:
	std::filesystem::path m_Filesystem;
};
#include "utils.h"

#include <fstream>

namespace utils {

	std::string readEntireFileToStr(const std::filesystem::path& path) {
        std::ifstream f(path, std::ios::in | std::ios::binary);
        const auto sz = std::filesystem::file_size(path);
        std::string result(sz, '\0');
        f.read(result.data(), sz);
        return result;
	}

}


#include "filesystem.h"

// TODO: move to precompiled header
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

std::string Filesystem::readFile(const std::filesystem::path& path) {
	std::string result;
	HANDLE fileHandle = CreateFileA(path.string().c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (fileHandle != INVALID_HANDLE_VALUE) [[likely]] {
		LARGE_INTEGER fileSize;
		if (GetFileSizeEx(fileHandle, &fileSize)) [[likely]] {
			result.resize(fileSize.QuadPart + 1);
			DWORD bytesRead;
			if (ReadFile(fileHandle, &result[0], fileSize.QuadPart, &bytesRead, 0) 
				&& fileSize.QuadPart == bytesRead) [[likely]] {
				// File read successfully
			}
			else [[unlikely]] {
				// log
			}
		}
		else [[unlikely]] {
			// log
		}
	}
	else [[unlikely]] {
		// log
	}
	return result;
}

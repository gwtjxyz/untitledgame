#include "filesystem.hpp"

#include <cstdio>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

std::vector<char> read_file(const char * const filename) {
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		SE_FATAL("failed to open file!");
	}
	size_t fileSize = (size_t) file.tellg();
	std::vector<char> buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}

std::vector<char> read_file_fs(const char * const filename) {
	auto path = fs::path(filename);

	if (!std::filesystem::exists(path)) {
		SE_FATAL("Failed to open file %s", path.filename().c_str());
	}

	return read_file(path.c_str());
}
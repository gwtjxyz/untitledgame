#include "filesystem.hpp"

#include <cstdio>
#include <fstream>

std::vector<char> read_file(const char * const pathToProjectDir, const char * const pathFromProjectDir) {
	// The rationale for the piece of code below is that for some reason relative
	// file paths on Windows act differently to relative file paths on Linux
	// it's a bit annoying to have to supply both arguments if you don't even
	// necessarily need them, but I can't come up with a cleaner solution currently
	char fullFilePath[1000];
#ifdef _WIN32
	SE_SPRINTF(fullFilePath, "%s%s", pathToProjectDir, pathFromProjectDir);
#else // __linux__
	SE_SPRINTF(fullFilePath, "%s", pathFromProjectDir);
	SE_UNUSED(pathToProjectDir);
#endif

	std::ifstream file(fullFilePath, std::ios::ate | std::ios::binary);

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


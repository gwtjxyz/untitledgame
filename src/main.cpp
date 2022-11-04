#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include "framework/common/logging.hpp"
#include <iostream>

int main() {
    if (!glfwInit()) {
        std::cerr << "ERROR Failed to initialize GLFW! Aborting..." << std::endl;
    }

    if (!glfwVulkanSupported()) {
        std::cerr << "ERROR Vulkan is not supported! Aborting..." << std::endl;
        return -1;
    }

    const char * str = "works!";
    SE_TRACE("Hope this %s", str);
    SE_WARN("This is a warning.");
    SE_ERROR("This is an error.");
    SE_TRACE("Trace at %s:%d", __FILE__, __LINE__);
    SE_FATAL("Something VERY BAD just happened.");

    return 0;
}

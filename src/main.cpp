#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include "famework/common/assert.hpp"

#include <iostream>

int main() {
    if (!glfwInit()) {
        std::cerr << "ERROR Failed to initialize GLFW! Aborting..." << std::endl;
    }

    if (!glfwVulkanSupported()) {
        std::cerr << "ERROR Vulkan is not supported! Aborting..." << std::endl;
        return -1;
    }

    return 0;
}

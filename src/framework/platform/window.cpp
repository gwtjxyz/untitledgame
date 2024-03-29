#include "window.hpp"
#include "GLFW/glfw3.h"

static void glfw_error_callback(int code, const char * desc) {
    printf("GLFW Error %d: %s\n", code, desc);
    SE_ERROR("GLFW Error %d: %s", code, desc);
}

i32 se::glfw_init() {
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit()) {
        SE_FATAL("ERROR Failed to initialize GLFW! Aborting...");
        return se::ERR;
    }

    if (!glfwVulkanSupported()) {
        SE_FATAL("ERROR Vulkan is not supported! Aborting...");
        return se::ERR;
    }

    return se::OK;
}

GLFWwindow * se::window_init() {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow * window = glfwCreateWindow(se::WINDOW_WIDTH, se::WINDOW_HEIGHT, se::WINDOW_TITLE, nullptr, nullptr);

    if (!window)
        SE_FATAL("ERROR Failed to create GLFW window! Aborting...");
    
    return window;
}

void se::window_cleanup() {
    glfwTerminate();
}

void se::window_loop(GLFWwindow * window, VkRenderer & renderer) {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        renderer.draw();
        // TODO other logic
    }
}

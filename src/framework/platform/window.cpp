#include "window.hpp"


i32 se::glfw_init() {
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
    GLFWwindow * window = glfwCreateWindow(se::WINDOW_WIDTH, se::WINDOW_HEIGHT, se::WINDOW_TITLE, nullptr, nullptr);

    if (!window)
        SE_FATAL("ERROR Failed to create GLFW window! Aborting...");
    
    return window;
}

void se::window_cleanup() {
    glfwTerminate();
}

void se::window_loop(GLFWwindow * window) {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        // TODO other logic
    }
}

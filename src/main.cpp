#include "framework/core.hpp"
#include "framework/platform/window.hpp"
#include "framework/render/vk_renderer.hpp"

int main() {
    if (se::glfw_init() != se::OK) {
        return -1;
    }

    auto window = se::window_init();

    if (!window) {
        return -1;
    }

    VkRenderer renderer("Untitled game", window);

    se::window_loop(window);

    se::window_cleanup();

    return 0;
}

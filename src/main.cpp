#include "framework/core.hpp"
#include "framework/platform/window.hpp"

int main() {
    if (se::glfw_init() != se::OK) {
        return -1;
    }

    auto window = se::window_init();

    if (!window) {
        return -1;
    }

    se::window_loop(window);

    se::window_cleanup();

    return 0;
}

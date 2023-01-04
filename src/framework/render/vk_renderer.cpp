#include "vk_renderer.hpp"

VkRenderer::VkRenderer(const char * const applicationName) {
    VkApplicationInfo applicationInfo = {
            VK_STRUCTURE_TYPE_APPLICATION_INFO,
            nullptr,
            applicationName,
            VK_API_VERSION_1_2,
            se::WINDOW_TITLE,
            VK_MAKE_VERSION(1, 0, 0),
            VK_API_VERSION_1_2
    };

    VkInstanceCreateInfo instanceCreateInfo = {
            VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            nullptr,
            0,
            &applicationInfo,
            0,
            nullptr,
            0,
            nullptr
    };

    SE_ASSERT(vkCreateInstance(&instanceCreateInfo, nullptr, &m_Instance) == VK_SUCCESS, "Could not create Vulkan instance!");
}

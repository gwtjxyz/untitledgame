#pragma once
//----------------------------------------------------------------------------------------
/**
 * \file       vk_renderer.hpp
 * \author     Yury Udavichenka
 * \date       04/01/2023
 * \brief      Vulkan renderer class
 *
 *  Meant to be a general-purpose abstraction of Vulkan's boilerplate
 *  that can be easily set up and used for drawing whatever we want to draw
 *
*/
//----------------------------------------------------------------------------------------
#include "../core.hpp"
#include "../platform/window.hpp"

#include <vector>

class VkRenderer {
public:
    VkRenderer(const char * applicationName);
    virtual ~VkRenderer();
private:
    VkInstance m_Instance;
    VkDebugUtilsMessengerEXT m_DebugMessenger;

    bool check_validation_layer_support(const char * const layerName);
    void setup_debug_messenger();
    VkResult create_debug_utils_messenger_EXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT * createInfo,
                                              const VkAllocationCallbacks * allocator, VkDebugUtilsMessengerEXT * debugMessenger);
};

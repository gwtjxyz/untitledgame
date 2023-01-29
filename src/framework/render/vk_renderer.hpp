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
#include <optional>

class VkRenderer {
public:
    VkRenderer(const char * applicationName);
    virtual ~VkRenderer();
private:
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;

        bool is_complete() {
            return graphicsFamily.has_value();
        }
    };
    VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
    VkInstance m_Instance;
    VkDevice m_Device;
    VkQueue m_GraphicsQueue;
    VkDebugUtilsMessengerEXT m_DebugMessenger;
    std::vector<const char *> m_ValidationLayers;

    void create_instance(const char * const applicationName);
    void pick_physical_device();
    void setup_debug_messenger();
    void create_logical_device();
    VkResult create_debug_utils_messenger_EXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT * createInfo,
                                              const VkAllocationCallbacks * allocator, VkDebugUtilsMessengerEXT * debugMessenger);
    QueueFamilyIndices find_queue_families(VkPhysicalDevice device);

    bool check_validation_layer_support(const char * const layerName);
    bool is_device_suitable(VkPhysicalDevice device);
};

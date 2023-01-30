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
    VkRenderer(const char * applicationName, GLFWwindow * window);
    virtual ~VkRenderer();
private:
    struct QueueFamilyIndices {
        std::optional<u32> graphicsFamily;
        std::optional<u32> presentFamily;

        bool is_complete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities{};
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
    VkInstance m_Instance;
    VkDevice m_Device;
    VkQueue m_GraphicsQueue;
    VkQueue m_PresentQueue;
    VkSurfaceKHR m_Surface;

    VkSwapchainKHR m_SwapChain;
    std::vector<VkImage> m_SwapChainImages;
    std::vector<VkImageView> m_SwapChainImageViews;
    VkFormat m_SwapChainImageFormat;
    VkExtent2D m_SwapChainExtent;

    VkDebugUtilsMessengerEXT m_DebugMessenger;
    std::vector<const char *> m_ValidationLayers;
    std::vector<const char*> m_DeviceExtensions;

    void create_instance(const char * const applicationName);
    void pick_physical_device();
    void setup_debug_messenger();
    void create_logical_device();
    void create_surface(GLFWwindow * window);
    void create_swap_chain(GLFWwindow * window);
    void create_image_views();
    VkResult create_debug_utils_messenger_EXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT * createInfo,
                                              const VkAllocationCallbacks * allocator, VkDebugUtilsMessengerEXT * debugMessenger);
    QueueFamilyIndices find_queue_families(VkPhysicalDevice device);
    SwapChainSupportDetails query_swap_chain_support(VkPhysicalDevice device);

    VkSurfaceFormatKHR choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR> & availableFormats);
    VkPresentModeKHR choose_swap_chain_present_mode(const std::vector<VkPresentModeKHR> & availablePresentModes);
    VkExtent2D choose_swap_extent(const VkSurfaceCapabilitiesKHR & capabilities, GLFWwindow * window);

    bool check_validation_layer_support(const char * const layerName);
    bool is_device_suitable(VkPhysicalDevice device);
    bool check_device_extension_support(VkPhysicalDevice device);
};

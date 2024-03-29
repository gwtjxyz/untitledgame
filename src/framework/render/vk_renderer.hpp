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

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif // !GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <optional>

class VkRenderer {
public:
    VkRenderer() = delete;
    VkRenderer(const char * applicationName, GLFWwindow * window);
    virtual ~VkRenderer();

    void draw();
    void trigger_framebuffer_resize();
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

    GLFWwindow * m_Window;

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

    VkRenderPass m_RenderPass;
    VkPipelineLayout m_PipelineLayout;
    VkPipeline m_GraphicsPipeline;

    std::vector<VkFramebuffer> m_SwapChainFramebuffers;

    u32 m_CurrentFrame = 0;
    const int MAX_FRAMES_IN_FLIGHT = 2;
    VkCommandPool m_CommandPool;
    std::vector<VkCommandBuffer> m_CommandBuffers;

    std::vector<VkSemaphore> m_ImageAvailableSemaphores;
    std::vector<VkSemaphore> m_RenderFinishedSemaphores;
    std::vector<VkFence> m_InFlightFences;

    bool m_FramebufferResized = false;

    VkDebugUtilsMessengerEXT m_DebugMessenger;
    VkDebugReportCallbackEXT m_DebugReportCallback;
    std::vector<const char *> m_ValidationLayers;
    std::vector<const char *> m_DeviceExtensions;

    void setup_swap_chain();

    void cleanup_swap_chain();

    void recreate_swap_chain();

    void record_command_buffer(VkCommandBuffer commandBuffer, u32 imageIndex);

    VkResult create_debug_utils_messenger_EXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT * createInfo,
                                              const VkAllocationCallbacks * allocator, VkDebugUtilsMessengerEXT * debugMessenger);
    QueueFamilyIndices find_queue_families(VkPhysicalDevice device);
    SwapChainSupportDetails query_swap_chain_support(VkPhysicalDevice device);

    VkSurfaceFormatKHR choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR> & availableFormats);
    VkPresentModeKHR choose_swap_chain_present_mode(const std::vector<VkPresentModeKHR> & availablePresentModes);
    VkExtent2D choose_swap_extent(const VkSurfaceCapabilitiesKHR & capabilities, GLFWwindow * window);

    VkShaderModule create_shader_module(const std::vector<char> & spirvCode);

    bool check_validation_layer_support(const char * const layerName);
    bool is_device_suitable(VkPhysicalDevice device);
    bool check_device_extension_support(VkPhysicalDevice device);
};

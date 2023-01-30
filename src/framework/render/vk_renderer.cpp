#include "vk_renderer.hpp"

#include <algorithm>
#include <cstring>
#include <limits>
#include <set>
#include <string>

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT severity,
        VkDebugUtilsMessageTypeFlagsEXT type,
        const VkDebugUtilsMessengerCallbackDataEXT * callbackData,
        void * userData) {
    if (severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        SE_TRACE(callbackData->pMessage);
    }

    return VK_FALSE;
}

VkRenderer::VkRenderer(const char * const appName, GLFWwindow * window) {
    create_instance(appName);
    setup_debug_messenger();
    create_surface(window);
    pick_physical_device();
    create_logical_device();
    create_swap_chain(window);
    create_image_views();
}

void VkRenderer::create_instance(const char * const appName) {
    VkApplicationInfo applicationInfo = {
        VK_STRUCTURE_TYPE_APPLICATION_INFO,
        nullptr,
        appName,
        VK_MAKE_VERSION(1, 0, 0),
        se::WINDOW_TITLE,
        VK_MAKE_VERSION(1, 0, 0),
        VK_API_VERSION_1_3
    };

    // TODO make this changeable via compiler parameters?
    m_ValidationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    m_DeviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    // TODO extract into separate function
    for (auto layer : m_ValidationLayers) {
        SE_ASSERT(check_validation_layer_support(layer), "Required layer %s is not available!", layer);
    }

    // TODO expand this with extra extensions
    u32 extensionCount;
    const char ** extensions = glfwGetRequiredInstanceExtensions(&extensionCount);
    std::vector<const char *> extensionsVector(extensions, extensions + extensionCount);
    // TODO make boolean flag for this
    extensionsVector.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    VkInstanceCreateInfo instanceCreateInfo = {
        VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        nullptr,
        0,
        &applicationInfo,
        static_cast<u32>(m_ValidationLayers.size()),
        m_ValidationLayers.data(),
        static_cast<u32>(extensionsVector.size()),
        extensionsVector.data()
    };

    SE_ASSERT(vkCreateInstance(&instanceCreateInfo, nullptr, &m_Instance) == VK_SUCCESS,
              "Could not create Vulkan instance!");
}

void VkRenderer::pick_physical_device() {
    u32 deviceCount = 0;
    vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        SE_FATAL("Failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data());

    for (const auto & device : devices) {
        if (is_device_suitable(device)) {
            m_PhysicalDevice = device;
            break;
        }
    }

    if (m_PhysicalDevice == VK_NULL_HANDLE) {
        SE_FATAL("Failed to find a suitable GPU!");
    }
}

// TODO add some condition for not enabling this
void VkRenderer::setup_debug_messenger() {
    VkDebugUtilsMessageSeverityFlagsEXT messageSeverityFlags =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

    VkDebugUtilsMessageTypeFlagsEXT messageTypeFlags =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

    VkDebugUtilsMessengerCreateInfoEXT createInfo = {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .messageSeverity = messageSeverityFlags,
            .messageType = messageTypeFlags,
            .pfnUserCallback = debug_callback,
            .pUserData = nullptr
    };

    if (create_debug_utils_messenger_EXT(m_Instance, &createInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS) {
        SE_FATAL("Failed to set up debug messenger!");
    }
}

void VkRenderer::create_logical_device() {
    QueueFamilyIndices indices = find_queue_families(m_PhysicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<u32> uniqueQueueFamilies = {
        indices.graphicsFamily.value(),
        indices.presentFamily.value()
    };

    float queuePriority = 1.0f;
    for (u32 queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = (u32) (queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = (u32) (m_DeviceExtensions.size());
    createInfo.ppEnabledExtensionNames = m_DeviceExtensions.data();
   
    // TODO add a on/off switch for validation layers
    // that or remove this completely, it's legacy stuff anyway
    createInfo.enabledLayerCount = (u32) (m_ValidationLayers.size());
    createInfo.ppEnabledLayerNames = m_ValidationLayers.data();

    if (vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device) != VK_SUCCESS) {
        SE_FATAL("Failed to create logical device!");
    }

    vkGetDeviceQueue(m_Device, indices.graphicsFamily.value(), 0, &m_GraphicsQueue);
    vkGetDeviceQueue(m_Device, indices.presentFamily.value(), 0, &m_PresentQueue);
}

void VkRenderer::create_surface(GLFWwindow * window) {
    if (glfwCreateWindowSurface(m_Instance, window, nullptr, &m_Surface) != VK_SUCCESS) {
        SE_FATAL("Failed to create window surface!");
    }
}

void VkRenderer::create_swap_chain(GLFWwindow * window) {
    SwapChainSupportDetails swapChainSupport = query_swap_chain_support(m_PhysicalDevice);
    VkSurfaceFormatKHR surfaceFormat = choose_swap_surface_format(swapChainSupport.formats);
    VkPresentModeKHR presentMode = choose_swap_chain_present_mode(swapChainSupport.presentModes);
    VkExtent2D extent = choose_swap_extent(swapChainSupport.capabilities, window);

    u32 imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    // TODO change to allow post-processing later on (VK_IMAGE_USAGE_TRANSFER_DST_BIT)
    VkSwapchainCreateInfoKHR createInfo = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = m_Surface,
        .minImageCount = imageCount,
        .imageFormat = surfaceFormat.format,
        .imageColorSpace = surfaceFormat.colorSpace,
        .imageExtent = extent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
    };

    QueueFamilyIndices indices = find_queue_families(m_PhysicalDevice);
    u32 queueFamilyIndices[] = {
        indices.graphicsFamily.value(),
        indices.presentFamily.value()
    };

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(m_Device, &createInfo, nullptr, &m_SwapChain) != VK_SUCCESS) {
        SE_FATAL("Failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(m_Device, m_SwapChain, &imageCount, nullptr);
    m_SwapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(m_Device, m_SwapChain, &imageCount, m_SwapChainImages.data());

    m_SwapChainImageFormat = surfaceFormat.format;
    m_SwapChainExtent = extent;
}

void VkRenderer::create_image_views() {
    m_SwapChainImageViews.resize(m_SwapChainImages.size());
    for (size_t i = 0; i < m_SwapChainImages.size(); ++i) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = m_SwapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = m_SwapChainImageFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(m_Device, &createInfo, nullptr, &m_SwapChainImageViews[i]) != VK_SUCCESS) {
            SE_FATAL("Failed to create image views!");
        }
    }
}

VkResult VkRenderer::create_debug_utils_messenger_EXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *createInfo,
                                                      const VkAllocationCallbacks *allocator, VkDebugUtilsMessengerEXT *debugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, createInfo, allocator, debugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

VkRenderer::QueueFamilyIndices VkRenderer::find_queue_families(VkPhysicalDevice device) {
    QueueFamilyIndices indices;
    u32 queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto & queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_Surface, &presentSupport);
        if (presentSupport) {
            indices.presentFamily = i;
        }

        if (indices.is_complete()) {
            break;
        }

        i++;
    }
    return indices;
}

VkRenderer::SwapChainSupportDetails VkRenderer::query_swap_chain_support(VkPhysicalDevice device) {
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_Surface, &details.capabilities);

    u32 formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &formatCount, nullptr);
    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &formatCount, details.formats.data());
    }

    u32 presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &presentModeCount, nullptr);
    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

VkSurfaceFormatKHR VkRenderer::choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR> & availableFormats) {
    for (const auto & af : availableFormats) {
        if (af.format == VK_FORMAT_B8G8R8A8_SRGB &&
            af.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return af;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR VkRenderer::choose_swap_chain_present_mode(const std::vector<VkPresentModeKHR> & availablePresentModes) {
    // Mailbox = triple buffering, FIFO = sort of like vsync
    for (const auto & apm : availablePresentModes) {
        if (apm == VK_PRESENT_MODE_MAILBOX_KHR) {
            return apm;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VkRenderer::choose_swap_extent(const VkSurfaceCapabilitiesKHR & capabilities, GLFWwindow * window) {
    if (capabilities.currentExtent.width != (std::numeric_limits<u32>::max)()) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        VkExtent2D actualExtent = {
            (u32) width,
            (u32) height
        };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

bool VkRenderer::check_validation_layer_support(const char * const layerName) {
    u32 availableLayerCount;
    vkEnumerateInstanceLayerProperties(&availableLayerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(availableLayerCount);
    vkEnumerateInstanceLayerProperties(&availableLayerCount, availableLayers.data());

    bool layerFound = false;

    for (const auto & layerProperties : availableLayers) {
        if (strcmp(layerName, layerProperties.layerName) == 0) {
            layerFound = true;
            break;
        }
    }

    return layerFound;
}

// TODO implement this
bool VkRenderer::is_device_suitable(VkPhysicalDevice device) {
    QueueFamilyIndices indices = find_queue_families(device);

    bool extensionsSupported = check_device_extension_support(device);

    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = query_swap_chain_support(device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return indices.is_complete() && extensionsSupported && swapChainAdequate;
}

bool VkRenderer::check_device_extension_support(VkPhysicalDevice device) {
    u32 extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(m_DeviceExtensions.begin(), m_DeviceExtensions.end());

    for (const auto & extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

VkRenderer::~VkRenderer() {
    // TODO parametrize
    for (auto view : m_SwapChainImageViews) {
        vkDestroyImageView(m_Device, view, nullptr);
    }
    vkDestroySwapchainKHR(m_Device, m_SwapChain, nullptr);
    vkDestroyDevice(m_Device, nullptr);
    auto destroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_Instance, "vkDestroyDebugUtilsMessengerEXT");
    if (destroyDebugUtilsMessengerEXT != nullptr) {
        destroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
    }
    vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
    vkDestroyInstance(m_Instance, nullptr);
}

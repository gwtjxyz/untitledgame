#include "vk_renderer.hpp"

#include <cstring>

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

VkRenderer::VkRenderer(const char * const applicationName) {
    VkApplicationInfo applicationInfo = {
            VK_STRUCTURE_TYPE_APPLICATION_INFO,
            nullptr,
            applicationName,
            VK_MAKE_VERSION(1, 0, 0),
            se::WINDOW_TITLE,
            VK_MAKE_VERSION(1, 0, 0),
            VK_API_VERSION_1_2
    };

    // TODO make this changeable via compiler parameters?
    const std::vector<const char *> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
    };

    // TODO extract into separate function
    for (auto layer : validationLayers) {
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
            static_cast<uint32_t>(validationLayers.size()),
            validationLayers.data(),
            static_cast<uint32_t>(extensionsVector.size()),
            extensionsVector.data()
    };

    SE_ASSERT(vkCreateInstance(&instanceCreateInfo, nullptr, &m_Instance) == VK_SUCCESS,
              "Could not create Vulkan instance!");

    setup_debug_messenger();
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

VkResult VkRenderer::create_debug_utils_messenger_EXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *createInfo,
                                                      const VkAllocationCallbacks *allocator, VkDebugUtilsMessengerEXT *debugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, createInfo, allocator, debugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

VkRenderer::~VkRenderer() {
    // TODO parametrize
    auto destroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_Instance, "vkDestroyDebugUtilsMessengerEXT");
    if (destroyDebugUtilsMessengerEXT != nullptr) {
        destroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
    }
    vkDestroyInstance(m_Instance, nullptr);
}

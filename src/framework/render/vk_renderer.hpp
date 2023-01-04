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

class VkRenderer {
public:
    VkRenderer(const char * applicationName);
    virtual ~VkRenderer() = default;
private:
    VkInstance m_Instance;
};

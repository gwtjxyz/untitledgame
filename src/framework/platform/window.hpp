//----------------------------------------------------------------------------------------
/**
 * \file       window.hpp
 * \author     Yury Udavichenka
 * \date       22/10/2022
 * \brief      Window functionality
 *
 *  Header containing functions for GLFW window creation and management
 *
*/
//----------------------------------------------------------------------------------------
#pragma once

#include "../core.hpp"

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif // !GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace se {

i32 glfw_init();

GLFWwindow * window_init();

void window_cleanup();

void window_loop(GLFWwindow * window);

} // namespace se
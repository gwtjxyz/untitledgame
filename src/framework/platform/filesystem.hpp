#pragma once
//----------------------------------------------------------------------------------------
/**
* \file       filesystem.hpp
* \author     Yury Udavichenka
* \date       30/01/2023
* \brief      File system functionality
*
*  Header containing functions for interacting with the OS file system
*
*/
//----------------------------------------------------------------------------------------
#include "../core.hpp"

#include <vector>

// if we are on Windows, we append the path to project dir to the path from project dir
// if we are on Linux, we just use the path from project dir
std::vector<char> read_file(const char * const pathToProjectDir, const char * const pathFromProjectDir);

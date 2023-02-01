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

std::vector<char> read_file(const char * const filename);
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
#include <filesystem>

std::vector<char> read_file(const char * const filename);

// filesystem version, may be more portable
std::vector<char> read_file_fs(const char * const filename);
cmake_minimum_required(VERSION 3.23)
project(untitledgame)

set(CMAKE_CXX_STANDARD 20)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

find_package(Vulkan REQUIRED FATAL_ERROR)

include_directories(lib/glfw/include)

add_executable(untitledgame src/main.cpp)

add_subdirectory(lib/glfw)

target_include_directories(${PROJECT_NAME}
        PUBLIC lib/glfw/include
        )

target_link_libraries(${PROJECT_NAME}
        PRIVATE glfw
        )

cmake_minimum_required(VERSION 3.23)
project(untitledgame)

set(CMAKE_CXX_STANDARD 20)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

add_executable(untitledgame src/main.cpp)

find_package(Vulkan REQUIRED FATAL_ERROR)

include_directories(lib/glfw/include)

add_subdirectory(lib/glfw)

if(MSVC)    # Windows
    target_include_directories(${PROJECT_NAME}
            PUBLIC ${VULKAN_SDK}/Include
            )
endif()
    target_include_directories(${PROJECT_NAME}
            PUBLIC lib/glfw/include
            )

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        Vulkan::Vulkan glfw
        )

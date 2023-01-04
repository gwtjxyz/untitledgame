cmake_minimum_required(VERSION 3.23)
project(untitledgame)

set(CMAKE_CXX_STANDARD 20)

# Compiler flags
# TODO other compilers
if(MSVC)
    set(MSVC_COVERAGE_COMPILE_FLAGS "/W4")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MSVC_COVERAGE_COMPILE_FLAGS}")
endif()

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

add_executable(untitledgame src/main.cpp src/framework/platform/window.cpp)

find_package(Vulkan REQUIRED)

include_directories(lib/glfw/include)

add_subdirectory(lib/glfw)

if(MSVC)    # Windows
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /w14640)

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

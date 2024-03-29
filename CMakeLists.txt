cmake_minimum_required(VERSION 2.8.10)

add_subdirectory(test)

# Include required dependencies
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
pkg_search_module(GLEW REQUIRED glew)
pkg_search_module(LIBPNG REQUIRED libpng)

# Add all include directories
include_directories(include)
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${LIBPNG_INCLUDE_DIRS})

# Create main project
project(opengl)

# Add all required sources
add_executable(opengl src/main.cpp
    src/Shader.cpp src/WorldChunk.cpp src/World.cpp src/png_texture_load.cpp)

# Link external libraries.
target_link_libraries(opengl ${CMAKE_DL_LIBS})
target_link_libraries(opengl ${GLEW_LIBRARIES})
target_link_libraries(opengl ${GLFW_LIBRAIRES})
target_link_libraries(opengl ${GLFW_STATIC_LIBRARIES})
target_link_libraries(opengl ${LIBPNG_LIBRARIES})

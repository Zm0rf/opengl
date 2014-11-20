cmake_minimum_required(VERSION 2.8.10)
project(opengl)
add_library(test SHARED src/test.cpp)

# add_subdirectory(test)

include_directories(include)

add_executable(opengl src/main.cpp)

target_link_libraries(opengl ${CMAKE_DL_LIBS})


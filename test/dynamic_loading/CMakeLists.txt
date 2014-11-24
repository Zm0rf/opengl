cmake_minimum_required(VERSION 2.8.10)

# enable_testing()
# Includes
include_directories(../)
include_directories(../../include)

# find_package(GTest REQUIRED)
# include_directories(${GTEST_INCLUDE_DIR})
# target_link_libraries(${PROJECT_TEST_NAME} ${GTEST_LIBRARY} ${GTEST_MAIN_LIBRARY})

# Defines
add_definitions(-DTEST_LIBRARY_PATH="test/dynamic_loading/libdyn_loaded_binary.so")

# Test dynamic loading
project(run_test)
add_executable(run_test main.cpp)
target_link_libraries(run_test ${CMAKE_DL_LIBS})

add_library(dyn_loaded_binary SHARED library.cpp)

cmake_minimum_required(VERSION 3.8)

# Project Info
project(Mhysa VERSION 0.1.0)

# Language Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

# Headers & Sources
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/lib
)

# Static libraries
set(WITH_GFLAGS false)
add_subdirectory(lib/glog)
add_subdirectory(lib/googletest)

# Targets
add_executable(Debug_AARCH64
        src/Main.cpp
        src/Algorithms/Sorting.cpp
        src/Formatting/NumberFormatting.cpp)
target_link_libraries(Debug_AARCH64 PRIVATE glog::glog)

add_executable(Release_AARCH64
        src/Main.cpp
        src/Algorithms/Sorting.cpp
        src/Formatting/NumberFormatting.cpp)
target_link_libraries(Release_AARCH64 PRIVATE glog::glog)

# Compiler flags
target_compile_options(
    Debug_AARCH64
    PRIVATE
    -O0 -DDEBUG -Wall -Wextra -Wpedantic -g
)
target_compile_options(
    Release_AARCH64
    PRIVATE
    -O3 -DNDEBUG -fomit-frame-pointer -flto
)

# Test settings
enable_testing()
add_executable(AllTest_AARCH64
        src/Algorithms/Sorting.cpp
        src/Formatting/NumberFormatting.cpp
        tests/Algorithms/InsertionSortTest.cpp
        tests/Algorithms/BubbleSortTest.cpp
        tests/Algorithms/MergeSortTest.cpp
        tests/Algorithms/HeapSortTest.cpp
        tests/Algorithms/QuickSortTest.cpp)
target_link_libraries(AllTest_AARCH64 PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(AllTest_AARCH64)
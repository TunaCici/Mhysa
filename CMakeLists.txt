cmake_minimum_required(VERSION 3.8)

# Project Info
project(Mhysa VERSION 0.2.0)

# CMake Output
set(CMAKE_VERBOSE_MAKEFILE ON)

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
add_executable(Mhysa_Experimental
        src/Main.cpp
        src/Experimental/Fibonacci.cpp)
target_link_libraries(Mhysa_Experimental PRIVATE glog::glog)

add_executable(Mhysa_Debug
        src/Main.cpp
        src/Algorithms/Sorting.cpp
        src/Formatting/NumberFormatting.cpp)
target_link_libraries(Mhysa_Debug PRIVATE glog::glog)

add_executable(Mhysa_Release
        src/Main.cpp
        src/Algorithms/Sorting.cpp
        src/Formatting/NumberFormatting.cpp)
target_link_libraries(Mhysa_Release PRIVATE glog::glog)

# Compiler flags for Debug & Experimental
if(MSVC)
    target_compile_options(Mhysa_Debug PRIVATE -O0 -DDEBUG -Wall -g)
    target_compile_options(Mhysa_Experimental PRIVATE -O0 -DDEBUG -DEXPERIMENTAL -Wall -g)
else()
    target_compile_options(Mhysa_Debug PRIVATE -O0 -DDEBUG -Wall -Wextra -Wpedantic -g)
    target_compile_options(Mhysa_Experimental PRIVATE -O0 -DDEBUG -DEXPERIMENTAL -Wall -Wextra -Wpedantic -g)
endif()

# Compiler flags for Release
target_compile_options(Mhysa_Release PRIVATE -O3 -DNDEBUG -fomit-frame-pointer -flto)

# Test settings
enable_testing()
add_executable(AllTest_AARCH64
        src/Algorithms/Sorting.cpp
        src/Formatting/NumberFormatting.cpp
        tests/Algorithms/InsertionSortTest.cpp
        tests/Algorithms/BubbleSortTest.cpp
        tests/Algorithms/MergeSortTest.cpp
        tests/Algorithms/HeapSortTest.cpp
        tests/Algorithms/QuickSortTest.cpp
        tests/DataStructures/StackTest.cpp
        tests/DataStructures/QueueTest.cpp
        tests/DataStructures/LinkedListTest.cpp
        tests/DataStructures/BinaryTreeTest.cpp
        tests/DataStructures/RedBlackTreeTest.cpp)
target_link_libraries(AllTest_AARCH64 PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(AllTest_AARCH64)
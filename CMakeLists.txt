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

set(SOURCE_LIST
    "src/Main.cpp"

    "include/Algorithms/Sorting.hpp"
    "src/Algorithms/Sorting.cpp"

    "include/Formatting/NumberFormatting.hpp"
    "src/Formatting/NumberFormatting.cpp"
)

# Static libraries
option(BUILD_TESTING OFF)
enable_testing()
add_subdirectory(lib/glog)

# Targets
add_executable(Debug_AARCH64 ${SOURCE_LIST})
target_link_libraries(Debug_AARCH64 PRIVATE glog::glog)


add_executable(Release_AARCH64 ${SOURCE_LIST})
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

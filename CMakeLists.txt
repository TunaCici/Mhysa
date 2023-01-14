cmake_minimum_required(VERSION 3.0.0)

# Projcet Info
project(Mhysa VERSION 0.1.0)

# C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

# Targets
add_executable(Debug_AARCH64
    src/Main.cpp
    src/Algorithms/Sorting.cpp
    src/Formatting/NumberFormatting.cpp
)

add_executable(Release_AARCH64
    src/Main.cpp
    src/Algorithms/Sorting.cpp
    src/Formatting/NumberFormatting.cpp
)

# Compiler Flags
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
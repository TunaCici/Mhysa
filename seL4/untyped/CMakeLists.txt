
include(${SEL4_TUTORIALS_DIR}/settings.cmake)
sel4_tutorials_regenerate_tutorial(${CMAKE_CURRENT_SOURCE_DIR})

cmake_minimum_required(VERSION 3.7.2)
project(untyped C ASM)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

sel4_tutorials_setup_roottask_tutorial_environment()

# Name the executable and list source files required to build it
add_executable(untyped src/main.c)

# List of libraries to link with the application.
target_link_libraries(untyped
    sel4
    muslc utils sel4tutorials
    sel4muslcsys sel4platsupport sel4utils sel4debug)

# Tell the build system that this application is the root task.
include(rootserver)
DeclareRootserver(untyped)

# utility CMake functions for the tutorials (not required in normal, non-tutorial applications)
set(FINISH_COMPLETION_TEXT "Failed to retype")
set(START_COMPLETION_TEXT "Failed to retype")
configure_file(${SEL4_TUTORIALS_DIR}/tools/expect.py ${CMAKE_BINARY_DIR}/check @ONLY)
include(simulation)
GenerateSimulateScript()


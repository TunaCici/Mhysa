# Mhysa
The name is a High Valerian word for 'mother' and comes from the popular TV show Game of Thrones. Like the name suggests, this project is aims to be 'the mother (helper)' to all my other projects.

It includes commonly used algorithms, data structures, scripts, formattings, data conversions, validators and etc. It is written mainly in C++20 and the build system is CMake 3.25.x.

I am using spdlog's logging library made by gabime.
[Spdlog](https://github.com/gabime/spdlog)

## Requirements
CMake 3.00 or newer

## Installation
1. Clone this repository
```console
$ git clone --depth 1 https://github.com/TunaCici/Mhysa.git
```

2. Make & cd into build directory inside Mhysa/
```console
$ mkdir build
$ cd build
```

3. Run CMake to generate the Project files
```console
$ cmake ..                          # Standard Makefile
$ cmake -G Xcode ..                 # For XCode
$ cmake -G "Visual Studio 17 2022"  # For Visual Studio 2022
```

## Usage
Currently none.

## Directory structure
```
|-- .vscode            <- VSCode extension settings (clangd)
|-- bin                <- Compiled binaries
|-- build              <- CMake build
|-- data               <- Project data
|-- docs               <- Documentation
|-- include            <- Header files
|   `-- Algorithms     <- Algorithm headers
|   `-- Formatting     <- Formatting headers
|   `-- Spdlog         <- Spdlog headers & source
|-- lib                <- Dynamic objects
|-- src                <- Source files
|   `-- Algorithms     <- Algorithm sources
|   `-- Formatting     <- Formatting sources
|   `-- Main.cpp       <- Main function (for testing)
|-- tests              <- Unit tests
|-- .gitignore         <- Good-old .gitignore
|-- CMakeLists.txt     <- CMake configuration
|-- README-Linux.md    <- Collection of Linux Commands
`-- README.md          <- Main README
```

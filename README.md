# Mhysa
The name is a High Valerian word for 'mother' and comes from the popular TV show Game of Thrones. Like the name suggests, this project is aims to be 'the mother (helper)' to all my other projects.

It includes commonly used algorithms, data structures, scripts, formattings, data conversions, validators and etc. It is written mainly in C++20 and the build system is CMake 3.25.x.

## External Libraries
Logging: [Latest Google Log](https://github.com/google/glog) \
Testing: [Latest Google Test](https://github.com/google/googletest)

## Requirements
CMake 3.25 or newer

## Installation
1. Download & Install CMake >= 3.25
```console
# Linux
$ sudo apt install cmake # (Debian)
$ sudo pacman -Syu cmake # (Arch)

# macOS
$ brew install cmake # (Using brew)
Or go to the link: https://cmake.org/download/ # (Using DMG File)

# Windows 
Or go to the link: https://cmake.org/download/ # (Using MSI Installer)
```

2. Download & Install C/C++ Compiler (Clang or GCC)
```console
# Linux (gcc)
$ sudo apt install build-essential

# macOS (clang)
$ xcode-select --install

# Windows (msvc)
Follow the link:: https://learn.microsoft.com/en-us/cpp/build/vscpp-step-0-installation/
```

3. Clone this repository
```console
$ git clone --depth 1 https://github.com/TunaCici/Mhysa.git

# Also clone the external libraries
$ cd Mhysa
$ git submodule init
$ git submodule update
```

4. Make & cd into build directory inside Mhysa/
```console
$ mkdir -p build
$ cd build
```

5. Run CMake to generate the Project files
```console
$ cmake ..                          # For Makefile
$ cmake -G Xcode ..                 # For XCode
$ cmake -G "Visual Studio 17 2022"  # For Visual Studio 2022
```

## Testing
1. Build the test target
```console
# Linux
$ make AllTest_AARCH64

# macOS
$ make AllTest_AARCH64 (Terminal)
TODO: Using the XCode IDE

# Windows
TODO: Using the Visual Studio 2022
```

2. Run the tests (it might take a few minutes)
```console
# Linux
$ ./AllTest_AARCH64

# macOS
$ ./AllTest_AARCH64 (Terminal)
# TODO: Using the XCode IDE

# Windows
TODO: Using the Visual Studio 2022
```

## Directory structure
```
|-- bin                <- Compiled binaries
|-- build              <- CMake build
|-- data               <- Project data
|-- docs               <- Documentation
|-- include            <- Header files
|   `-- Algorithms     <- Algorithm headers
|   `-- Formatting     <- Formatting headers
|-- lib                <- External Libraries
|   `-- glog           <- Google Logging (from git)
|   `-- googletest     <- Google Testing (from git)
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

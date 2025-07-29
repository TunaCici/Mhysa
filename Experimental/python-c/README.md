## Basic Usage

Below are the files that is needed to ideally embed a Python interpretter within a C program that also implements a built-in library called *cherry*:

* Python Interface Definition File (cherry.pyi): This defines the module that will be used by a Python script and implemented by the C program
* C Header File For The Module Definitions (cherry.h): This includes the Python.h and define the C API that will be implemented later on. a.k.a. C bindings
* C Source File For The Module Implmenetation (cherry.c): This implements the C API for the cherry module
* C Main Program: This will include the Python.h and initialise the CPython interpreter. Later on it will execute the script
* Python Script: This is the Python script that will import the module and do user stuff. The script file will be passed onto the C Main Program to be interpreted

## CPython: python3-config

CLI helper tool that outputs build options for python C/C++ embedding (or extensions).

It is used to tell the the C/C++ compiler how to build the program

Think include headers and static/dynamic libraries.

## CPython: Objects

See: https://docs.python.org/3/c-api/structures.html

## CPython: C Bindings

## CPython: API Stability

See: https://docs.python.org/3/c-api/stable.html#

## CPython: Thread Safety / Global Interpretter Lock (GIL)

See: https://docs.python.org/3/c-api/init.html#thread-state-and-the-global-interpreter-lock
See: https://tonybaloney.github.io/posts/sub-interpreter-web-workers.html

## CPython: Isolated Configuration

See: https://docs.python.org/3/c-api/init_config.html#init-isolated-conf

## CPython: Memory Management

## CPython: Exception Handling

## CPython: Licensing

See: https://wiki.python.org/moin/PythonSoftwareFoundationLicenseFaq

## cgo: Memory Management

## cgo: Directives (#cgo)

## cgo: pkg-config

## cgo: Cross-compilation

## cgo: Downsides

* No support for function pointers
* No support for macro functions
* No auto-complete support for GoLand (https://youtrack.jetbrains.com/issue/GO-4032/CGO-auto-complete-improvement-Go-references-to-C)
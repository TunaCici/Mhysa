package main

/*
#cgo pkg-config: python3
#include "bridge.h"
*/
import "C"
import (
	"fmt"
	"os"
	"runtime"
	"unsafe"
)

//export go_add
func go_add(self *C.PyObject, args **C.PyObject, nargs C.Py_ssize_t, kwnames *C.PyObject) *C.PyObject {
	if kwnames != nil && C.PyTuple_Size(kwnames) != 0 {
		cmsg := C.CString("add() takes no keyword arguments")
		defer C.free(unsafe.Pointer(cmsg))
		C.PyErr_SetString(C.PyExc_TypeError, cmsg)
		return nil
	}

	if nargs != 2 {
		cmsg := C.CString(fmt.Sprintf(
			"add() takes exactly 2 positional arguments (%d given)",
			int(nargs)))
		defer C.free(unsafe.Pointer(cmsg))
		C.PyErr_SetString(C.PyExc_TypeError, cmsg)
		return nil
	}

	argv := (*[1 << 30]*C.PyObject)(unsafe.Pointer(args))[:nargs:nargs]
	a := argv[0] // borrowed refs – no INCREF needed
	b := argv[1]

	return C.PyNumber_Add(a, b) // returns new ref or NULL with exception
}

func main() {
	runtime.LockOSThread() // CPython requires a fixed OS thread (?)

	C.register_maverick()
	C.Py_Initialize()
	defer C.Py_Finalize()

	if len(os.Args) > 1 {
		/* Run a user-supplied script file */
		path := C.CString(os.Args[1])
		mode := C.CString("r")
		defer C.free(unsafe.Pointer(path))
		defer C.free(unsafe.Pointer(mode))

		file := C.fopen(path, mode)
		if file == nil {
			panic("cannot open script file")
		}
		C.PyRun_SimpleFileExFlags(file, path, 1, nil)
	} else {
		fmt.Println("No script file provided, running interactive mode.")
		/* Run the interactive interpreter */
		C.PyRun_InteractiveLoopFlags(C.stdin, C.CString("<stdin>"), nil)
	}
}

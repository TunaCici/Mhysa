package main

/*
#cgo pkg-config: python3-embed
#include <Python.h>
#include "cherry.h"

static PyObject* go_PyModule_Create(struct PyModuleDef* def) {
	return PyModule_Create(def);
}
*/
import "C"
import (
	"fmt"
	"os"
	"unsafe"
)

// ---------- Exception Object Stubs ----------
var (
	//export CherryConnectionError
	CherryConnectionError *C.PyObject

	//export CherryTimeoutError
	CherryTimeoutError *C.PyObject
)

// ---------- Top-Level Function Placeholders ----------

//export cherry_connect
func cherry_connect(self, args, kwargs *C.PyObject) *C.PyObject {
	return C.Py_None
}

//export cherry_disconnect
func cherry_disconnect(self, args *C.PyObject) *C.PyObject {
	return C.Py_None
}

//export cherry_send_message
func cherry_send_message(self, args *C.PyObject) *C.PyObject {
	return C.Py_None
}

//export cherry_receive_message
func cherry_receive_message(self, args, kwargs *C.PyObject) *C.PyObject {
	return C.Py_None
}

//export cherry_get_last_error
func cherry_get_last_error(self, args *C.PyObject) *C.PyObject {
	return C.Py_None
}

// ---------- Session Methods ----------

//export CherrySession_new
func CherrySession_new(tp *C.PyTypeObject, args, kwargs *C.PyObject) *C.PyObject {
	return C.Py_None
}

//export CherrySession_init
func CherrySession_init(self *C.CherrySessionObject, args, kwargs *C.PyObject) C.int {
	return 0
}

//export CherrySession_dealloc
func CherrySession_dealloc(self *C.CherrySessionObject) {
	// no-op
}

//export CherrySession_start
func CherrySession_start(self *C.CherrySessionObject, args *C.PyObject) *C.PyObject {
	return C.Py_None
}

//export CherrySession_stop
func CherrySession_stop(self *C.CherrySessionObject, args *C.PyObject) *C.PyObject {
	return C.Py_None
}

//export CherrySession_is_active
func CherrySession_is_active(self *C.CherrySessionObject, args *C.PyObject) *C.PyObject {
	return C.Py_None
}

//export CherrySession_send
func CherrySession_send(self *C.CherrySessionObject, args *C.PyObject) *C.PyObject {
	return C.Py_None
}

//export CherrySession_receive
func CherrySession_receive(self *C.CherrySessionObject, args, kwargs *C.PyObject) *C.PyObject {
	return C.Py_None
}

//export CherrySession_set_timeout
func CherrySession_set_timeout(self *C.CherrySessionObject, args *C.PyObject) *C.PyObject {
	return C.Py_None
}

//export CherrySession_get_timeout
func CherrySession_get_timeout(self *C.CherrySessionObject, args *C.PyObject) *C.PyObject {
	return C.Py_None
}

// ---------- Logging ----------

//export cherry_set_log_level
func cherry_set_log_level(self, args *C.PyObject) *C.PyObject {
	return C.Py_None
}

//export cherry_get_log_level
func cherry_get_log_level(self, args *C.PyObject) *C.PyObject {
	return C.Py_None
}

func main() {
	if len(os.Args) < 2 {
		fmt.Fprintf(os.Stderr, "Usage: %s <script.py>\n", os.Args[0])
		os.Exit(1)
	}

	script := os.Args[1]

	// Prepare C strings
	cArgv0 := C.CString(os.Args[0])
	defer C.free(unsafe.Pointer(cArgv0))
	cScript := C.CString(script)
	defer C.free(unsafe.Pointer(cScript))
	cCherry := C.CString("cherry")
	defer C.free(unsafe.Pointer(cCherry))

	program := C.Py_DecodeLocale(cArgv0, nil)
	if program == nil {
		fmt.Fprintln(os.Stderr, "Fatal error: cannot decode argv[0]")
		os.Exit(1)
	}

	// Register the module
	C.register_module()
	C.Py_SetProgramName(program)
	C.Py_Initialize()

	fp := C.fopen(cScript, C.CString("r"))
	if fp == nil {
		fmt.Fprintf(os.Stderr, "Could not open file %s\n", script)
		C.Py_Finalize()
		C.PyMem_RawFree(unsafe.Pointer(program))
		os.Exit(1)
	}

	result := C.PyRun_SimpleFile(fp, cScript)
	C.fclose(fp)

	if result != 0 {
		fmt.Fprintln(os.Stderr, "Failed to execute Python script.")
	}

	C.Py_Finalize()
	C.PyMem_RawFree(unsafe.Pointer(program))
	os.Exit(int(result))
}

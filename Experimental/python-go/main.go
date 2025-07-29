package main

/*
#cgo pkg-config: python3-embed
#include <Python.h>
#include "cherry.h"
*/
import "C"
import (
	"fmt"
	"os"
	"time"
	"unsafe"
)

// ---------- Exception Object Stubs ----------
var (
	//export CherryConnectionError
	CherryConnectionError *C.PyObject

	//export CherryTimeoutError
	CherryTimeoutError *C.PyObject
)

//export cherry_connect
func cherry_connect(self, args, kwargs *C.PyObject) *C.PyObject {
	if C.PyTuple_Size(args) != 2 {
		C.PyErr_SetString(C.PyExc_TypeError, C.CString("Expected 2 arguments: host (str), port (int)"))
		return nil
	}

	host := getGoArgFromPyArgs(args, 0)
	port := getGoArgFromPyArgs(args, 1)

	timeout := getGoKwarg(kwargs, "timeout")

	fmt.Printf("cherry_connect called with host: %s, port: %d\n", host, port)
	fmt.Printf("Timeout: %v\n", timeout)

	return C.Py_True
}

//export cherry_disconnect
func cherry_disconnect(self, args *C.PyObject) *C.PyObject {
	fmt.Println("cherry_disconnect called")
	return C.Py_True
}

//export cherry_get_last_error
func cherry_get_last_error(self, args *C.PyObject) *C.PyObject {
	return C.Py_None
}

//export CherrySession_new
func CherrySession_new(tp *C.PyTypeObject, args, kwargs *C.PyObject) *C.PyObject {
	newObj := C.PyType_GenericNew(tp, args, kwargs)
	if newObj == nil {
		C.PyErr_SetString(C.PyExc_RuntimeError, C.CString("Failed to create CherrySession object"))
		return nil
	}

	return newObj
}

//export CherrySession_init
func CherrySession_init(self *C.CherrySessionObject, args, kwargs *C.PyObject) C.int {
	fmt.Println("CherrySession_init called")

	kwargsMap := getGoKwargsMap(kwargs)
	if len(kwargsMap) == 0 {
		C.PyErr_SetString(C.PyExc_TypeError, C.CString("Expected keyword arguments: user (str), password (str)"))
		return -1
	}

	user := kwargsMap["user"].(string)
	password := kwargsMap["password"].(string)

	self.session_id = 47

	fmt.Printf("CherrySession initialized with user:password = %s:%s\n", user, password)

	return 0
}

//export CherrySession_dealloc
func CherrySession_dealloc(self *C.CherrySessionObject) {
	// no-op
}

//export CherrySession_start
func CherrySession_start(self *C.CherrySessionObject, args *C.PyObject) *C.PyObject {
	self.active = true
	return C.Py_True
}

//export CherrySession_stop
func CherrySession_stop(self *C.CherrySessionObject, args *C.PyObject) *C.PyObject {
	self.active = false
	return C.Py_None
}

//export CherrySession_is_active
func CherrySession_is_active(self *C.CherrySessionObject, args *C.PyObject) *C.PyObject {
	return goValueToPyObject(self.active)
}

//export CherrySession_send
func CherrySession_send(self *C.CherrySessionObject, args *C.PyObject) *C.PyObject {
	bytes := getGoArgFromPyArgs(args, 0)
	if bytes == nil {
		C.PyErr_SetString(C.PyExc_TypeError, C.CString("Expected bytes argument"))
		return nil
	}

	fmt.Printf("CherrySession_send called with bytes: %s\n", bytes)

	return goValueToPyObject(len(bytes.([]byte)))
}

//export CherrySession_receive
func CherrySession_receive(self *C.CherrySessionObject, args, kwargs *C.PyObject) *C.PyObject {
	bufferSize := getGoKwarg(kwargs, "buffer_size")
	if bufferSize == nil {
		C.PyErr_SetString(C.PyExc_TypeError, C.CString("Expected keyword argument: buffer_size (int)"))
		return nil
	}

	// dummy buffer
	dummyBuffer := []byte("dummy data received from Go!")
	if bufferSize.(int) < len(dummyBuffer) {
		C.PyErr_SetString(C.PyExc_ValueError, C.CString("buffer_size is too small"))
		return nil
	}

	return goValueToPyObject(dummyBuffer)
}

//export cherry_set_log_level
func cherry_set_log_level(self, args *C.PyObject) *C.PyObject {
	return C.Py_None
}

//export cherry_get_log_level
func cherry_get_log_level(self, args *C.PyObject) *C.PyObject {
	return C.Py_None
}

func runPythonScript(script string) {
	C.init_cpython()

	cScript := C.CString(script)
	defer C.free(unsafe.Pointer(cScript))

	fp := C.fopen(cScript, C.CString("r"))
	if fp == nil {
		fmt.Fprintf(os.Stderr, "Could not open file %s\n", script)
		os.Exit(1)
	}
	defer C.fclose(fp)

	result := C.PyRun_SimpleFile(fp, cScript)
	if result != 0 {
		fmt.Fprintln(os.Stderr, "Failed to execute Python script.")
	}
	C.finalize_cpython()
}

func main() {
	if len(os.Args) < 2 {
		fmt.Fprintf(os.Stderr, "Usage: %s <script.py>\n", os.Args[0])
		os.Exit(1)
	}

	fmt.Println(C.sizeof_CherrySessionObject)

	go runPythonScript(os.Args[1])

	// Wait for the Python interpreter to finish
	for {
		time.Sleep(1 * time.Second)
		if C.Py_IsInitialized() == 0 {
			break
		}
	}
}

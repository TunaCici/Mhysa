package main

/*
#cgo pkg-config: python3-embed
#include <Python.h>
*/
import "C"
import (
	"fmt"
	"unsafe"
)

// get Py_Object type as string
func getPyObjectTypeAsString(obj *C.PyObject) string {
	if obj == nil {
		return "None"
	}

	cStr := C.PyObject_Str(obj)
	if cStr == nil {
		return "Unknown"
	}
	defer C.Py_DecRef(cStr)

	return C.GoString(C.PyUnicode_AsUTF8(cStr))
}

// getGoValueFromPyObject converts a Python object to its Go equivalent value (int, float64, string, bool, nil, []interface{}, map[interface{}]interface{}).
func getGoValueFromPyObject(obj *C.PyObject) interface{} {
	if obj == nil {
		return nil
	}

	t := C.PyObject_Type(obj)
	if t == nil {
		return nil
	}
	defer C.Py_DecRef(t)

	typeName := C.PyObject_Str(t)
	if typeName == nil {
		return nil
	}
	defer C.Py_DecRef(typeName)

	goTypeName := C.GoString(C.PyUnicode_AsUTF8(typeName))

	switch goTypeName {
	case "<class 'int'>":
		return int(C.PyLong_AsLong(obj))
	case "<class 'float'>":
		return float64(C.PyFloat_AsDouble(obj))
	case "<class 'str'>":
		cstr := C.PyUnicode_AsUTF8(obj)
		return C.GoString(cstr)
	case "<class 'bool'>":
		if C.PyObject_IsTrue(obj) == 1 {
			return true
		}
		return false
	case "<class 'list'>":
		size := int(C.PyList_Size(obj))
		result := make([]interface{}, size)
		for i := 0; i < size; i++ {
			item := C.PyList_GetItem(obj, C.Py_ssize_t(i))
			// PyList_GetItem returns a borrowed reference, do not DECREF
			result[i] = getGoValueFromPyObject(item)
		}
		return result
	case "<class 'dict'>":
		result := make(map[interface{}]interface{})
		items := C.PyDict_Items(obj)
		if items != nil {
			defer C.Py_DecRef(items)
			size := int(C.PyList_Size(items))
			for i := 0; i < size; i++ {
				pair := C.PyList_GetItem(items, C.Py_ssize_t(i))
				key := C.PyTuple_GetItem(pair, 0)
				value := C.PyTuple_GetItem(pair, 1)
				result[getGoValueFromPyObject(key)] = getGoValueFromPyObject(value)
			}
		}
		return result
	case "<class 'bytes'>":
		size := C.PyBytes_Size(obj)
		if size < 0 {
			return nil
		}
		data := C.PyBytes_AsString(obj)
		if data == nil {
			return nil
		}
		return C.GoBytes(unsafe.Pointer(data), C.int(size))
	case "<class 'NoneType'>":
		return nil
	default:
		// fallback: return string representation
		strObj := C.PyObject_Str(obj)
		if strObj != nil {
			defer C.Py_DecRef(strObj)
			return C.GoString(C.PyUnicode_AsUTF8(strObj))
		}
		return nil
	}
}

// getGoArgFromPyArgs extracts the argument at the given position from a Python tuple (args) and returns it as a Go type.
func getGoArgFromPyArgs(args *C.PyObject, pos int) interface{} {
	if args == nil {
		return nil
	}

	size := int(C.PyTuple_Size(args))
	if pos < 0 || pos >= size {
		return nil
	}
	item := C.PyTuple_GetItem(args, C.Py_ssize_t(pos))
	// PyTuple_GetItem returns a borrowed reference, do not DECREF
	return getGoValueFromPyObject(item)
}

// getGoKwarg extracts the value for a given key from a Python kwargs dict (*C.PyObject) and returns it as a Go type.
func getGoKwarg(kwargs *C.PyObject, key string) interface{} {
	if kwargs == nil || C.PyDict_Size(kwargs) == 0 {
		return nil
	}

	pyKey := C.PyUnicode_FromString(C.CString(key))
	defer C.Py_DecRef(pyKey)
	val := C.PyDict_GetItem(kwargs, pyKey)

	// PyDict_GetItem returns a borrowed reference, do not DECREF
	return getGoValueFromPyObject(val)
}

// getGoKwargsMap converts a Python kwargs dict (*C.PyObject) to a Go map[string]interface{}.
func getGoKwargsMap(kwargs *C.PyObject) map[string]interface{} {
	result := make(map[string]interface{})
	if kwargs == nil || C.PyDict_Size(kwargs) == 0 {
		return result
	}
	items := C.PyDict_Items(kwargs)
	if items != nil {
		defer C.Py_DecRef(items)
		size := int(C.PyList_Size(items))
		for i := 0; i < size; i++ {
			pair := C.PyList_GetItem(items, C.Py_ssize_t(i))
			keyObj := C.PyTuple_GetItem(pair, 0)
			valObj := C.PyTuple_GetItem(pair, 1)
			keyGo := getGoValueFromPyObject(keyObj)
			if keyStr, ok := keyGo.(string); ok {
				result[keyStr] = getGoValueFromPyObject(valObj)
			}
		}
	}
	return result
}

// goValueToPyObject converts a Go value to a corresponding Python object (*C.PyObject).
func goValueToPyObject(val interface{}) *C.PyObject {
	switch v := val.(type) {
	case nil:
		C.Py_IncRef(C.Py_None)
		return C.Py_None
	case int:
		return C.PyLong_FromLong(C.long(v))
	case int32:
		return C.PyLong_FromLong(C.long(v))
	case int64:
		return C.PyLong_FromLongLong(C.longlong(v))
	case float32:
		return C.PyFloat_FromDouble(C.double(v))
	case float64:
		return C.PyFloat_FromDouble(C.double(v))
	case string:
		return C.PyUnicode_FromString(C.CString(v))
	case bool:
		if v {
			return C.Py_True
		}
		return C.Py_False
	case []interface{}:
		size := C.Py_ssize_t(len(v))
		list := C.PyList_New(size)
		for i, elem := range v {
			item := goValueToPyObject(elem)
			C.PyList_SetItem(list, C.Py_ssize_t(i), item) // Steals reference
		}
		return list
	case map[interface{}]interface{}:
		dict := C.PyDict_New()
		for key, value := range v {
			pyKey := goValueToPyObject(key)
			pyValue := goValueToPyObject(value)
			C.PyDict_SetItem(dict, pyKey, pyValue)
			C.Py_DecRef(pyKey)
			C.Py_DecRef(pyValue)
		}
		return dict
	case []byte:
		size := C.Py_ssize_t(len(v))
		bytesObj := C.PyBytes_FromStringAndSize(C.CString(string(v)), size)
		if bytesObj == nil {
			return nil
		}
		return bytesObj
	default:
		// fallback: try string representation
		s := fmt.Sprintf("%v", v)
		return C.PyUnicode_FromString(C.CString(s))
	}
}

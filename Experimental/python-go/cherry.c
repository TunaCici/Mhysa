// cherry.c
// Dummy implementation for the cherry built-in Python module

#include "cherry.h"

// Exception objects
PyObject* CherryConnectionError;
PyObject* CherryTimeoutError;

// Class methods
static PyMethodDef CherrySession_methods[] = {
    {"start", (PyCFunction)CherrySession_start, METH_NOARGS, "Start session"},
    {"stop", (PyCFunction)CherrySession_stop, METH_NOARGS, "Stop session"},
    {"is_active", (PyCFunction)CherrySession_is_active, METH_NOARGS, "Check if active"},
    {"send", (PyCFunction)CherrySession_send, METH_VARARGS, "Send data"},
    {"receive", (PyCFunction)CherrySession_receive, METH_VARARGS | METH_KEYWORDS, "Receive data"},
    {"set_timeout", (PyCFunction)CherrySession_set_timeout, METH_VARARGS, "Set timeout"},
    {"get_timeout", (PyCFunction)CherrySession_get_timeout, METH_NOARGS, "Get timeout"},
    {NULL}  // Sentinel
};

// Class
static PyTypeObject CherrySessionType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "cherry.Session",
    .tp_basicsize = sizeof(CherrySessionObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Cherry Session object",
    .tp_methods = CherrySession_methods,
    .tp_new = CherrySession_new,
    .tp_init = (initproc)CherrySession_init,
    .tp_dealloc = (destructor)CherrySession_dealloc,
};

// Module definition
static PyMethodDef CherryMethods[] = {
    {"connect", (PyCFunction)cherry_connect, METH_VARARGS | METH_KEYWORDS, "Connect"},
    {"disconnect", cherry_disconnect, METH_NOARGS, "Disconnect"},
    {"send_message", cherry_send_message, METH_VARARGS, "Send message"},
    {"receive_message", (PyCFunction)cherry_receive_message, METH_VARARGS | METH_KEYWORDS, "Receive message"},
    {"get_last_error", cherry_get_last_error, METH_NOARGS, "Get last error"},
    {NULL, NULL, 0, NULL}
};

// Module definition structure
static struct PyModuleDef cherrymodule = {
    PyModuleDef_HEAD_INIT,
    "cherry",
    "Cherry built-in module",
    -1,
    CherryMethods
};

// Module initialization
PyMODINIT_FUNC PyInit_cherry(void) {
    PyObject* m;

    if (PyType_Ready(&CherrySessionType) < 0)
        return NULL;

    m = PyModule_Create(&cherrymodule);
    if (m == NULL)
        return NULL;

    // Add constants
    PyModule_AddIntConstant(m, "DEFAULT_TIMEOUT", DEFAULT_TIMEOUT);
    PyModule_AddIntConstant(m, "MAX_CONNECTIONS", MAX_CONNECTIONS);

    // Create exceptions
    CherryConnectionError = PyErr_NewException("cherry.ConnectionError", NULL, NULL);
    Py_XINCREF(CherryConnectionError);
    PyModule_AddObject(m, "ConnectionError", CherryConnectionError);

    CherryTimeoutError = PyErr_NewException("cherry.TimeoutError", CherryConnectionError, NULL);
    Py_XINCREF(CherryTimeoutError);
    PyModule_AddObject(m, "TimeoutError", CherryTimeoutError);

    // Add Session class to module
    Py_INCREF(&CherrySessionType);
    if (PyModule_AddObject(m, "Session", (PyObject *)&CherrySessionType) < 0) {
        Py_DECREF(&CherrySessionType);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}

void register_module() {
    PyImport_AppendInittab("cherry", PyInit_cherry);
}

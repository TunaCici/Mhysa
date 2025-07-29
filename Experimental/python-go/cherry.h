// cherry.h
// C Bindings for the Python built-in module 'cherry'

#ifndef CHERRY_H
#define CHERRY_H

#include <Python.h>
#include <stdbool.h>

#define DEFAULT_TIMEOUT 30
#define MAX_CONNECTIONS 100

extern PyObject* cherry_connect(PyObject* self, PyObject* args, PyObject* kwargs);
extern PyObject* cherry_disconnect(PyObject* self, PyObject* args);
extern PyObject* cherry_get_last_error(PyObject* self, PyObject* args);

extern PyObject* CherryConnectionError;
extern PyObject* CherryTimeoutError;

// Session class
typedef struct {
    PyObject_HEAD
    int session_id;
    PyObject* user;
    PyObject* password;
    bool active;
} CherrySessionObject;

// Session class methods
extern PyObject* CherrySession_new(PyTypeObject* type, PyObject* args, PyObject* kwargs);
extern int CherrySession_init(CherrySessionObject* self, PyObject* args, PyObject* kwargs);
extern void CherrySession_dealloc(CherrySessionObject* self);

extern PyObject* CherrySession_start(CherrySessionObject* self, PyObject* args);
extern PyObject* CherrySession_stop(CherrySessionObject* self, PyObject* args);
extern PyObject* CherrySession_is_active(CherrySessionObject* self, PyObject* args);
extern PyObject* CherrySession_send(CherrySessionObject* self, PyObject* args);
extern PyObject* CherrySession_receive(CherrySessionObject* self, PyObject* args, PyObject* kwargs);

typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_CRITICAL
} CherryLogLevel;

extern PyObject* cherry_set_log_level(PyObject* self, PyObject* args);
extern PyObject* cherry_get_log_level(PyObject* self, PyObject* args);

// C helpers
void init_cpython();
void finalize_cpython();

// Wrappers for C API macros
static int go_PyUnicode_Check(PyObject *obj) {
    return PyUnicode_Check(obj);
}

static int go_PyLong_Check(PyObject *obj) {
    return PyLong_Check(obj);
}

static int go_PyBytes_Check(PyObject *obj) {
    return PyBytes_Check(obj);
}

static int go_PyTuple_Check(PyObject *obj) {
    return PyTuple_Check(obj);
}

#endif // CHERRY_H
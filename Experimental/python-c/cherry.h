// cherry.h
// C Bindings for the Python built-in module 'cherry'

#ifndef CHERRY_H
#define CHERRY_H

#include <Python.h>

// Constants
#define DEFAULT_TIMEOUT 30
#define MAX_CONNECTIONS 100

// Function declarations
PyObject* cherry_connect(PyObject* self, PyObject* args, PyObject* kwargs);
PyObject* cherry_disconnect(PyObject* self, PyObject* args);
PyObject* cherry_send_message(PyObject* self, PyObject* args);
PyObject* cherry_receive_message(PyObject* self, PyObject* args, PyObject* kwargs);
PyObject* cherry_get_last_error(PyObject* self, PyObject* args);

// Exception Objects
extern PyObject* CherryConnectionError;
extern PyObject* CherryTimeoutError;

// Session Object Structure
typedef struct {
    PyObject_HEAD
    int session_id;
    double timeout;
    int is_active;
} CherrySessionObject;

// Session Methods
typedef struct {
    PyObject_HEAD
    CherrySessionObject* session;
} PyCherrySession;

// Session method declarations
PyObject* CherrySession_new(PyTypeObject* type, PyObject* args, PyObject* kwargs);
int CherrySession_init(CherrySessionObject* self, PyObject* args, PyObject* kwargs);
void CherrySession_dealloc(CherrySessionObject* self);

PyObject* CherrySession_start(CherrySessionObject* self, PyObject* args);
PyObject* CherrySession_stop(CherrySessionObject* self, PyObject* args);
PyObject* CherrySession_is_active(CherrySessionObject* self, PyObject* args);
PyObject* CherrySession_send(CherrySessionObject* self, PyObject* args);
PyObject* CherrySession_receive(CherrySessionObject* self, PyObject* args, PyObject* kwargs);
PyObject* CherrySession_set_timeout(CherrySessionObject* self, PyObject* args);
PyObject* CherrySession_get_timeout(CherrySessionObject* self, PyObject* args);

// LogLevel Enum
typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_CRITICAL
} CherryLogLevel;

// Logging functions
PyObject* cherry_set_log_level(PyObject* self, PyObject* args);
PyObject* cherry_get_log_level(PyObject* self, PyObject* args);

#endif // CHERRY_H
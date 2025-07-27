// cherry.h
// C Bindings for the Python built-in module 'cherry'

#ifndef CHERRY_H
#define CHERRY_H

#include <Python.h>

// Constants
#define DEFAULT_TIMEOUT 30
#define MAX_CONNECTIONS 100

// Function declarations
extern PyObject* cherry_connect(PyObject* self, PyObject* args, PyObject* kwargs);
extern PyObject* cherry_disconnect(PyObject* self, PyObject* args);
extern PyObject* cherry_send_message(PyObject* self, PyObject* args);
extern PyObject* cherry_receive_message(PyObject* self, PyObject* args, PyObject* kwargs);
extern PyObject* cherry_get_last_error(PyObject* self, PyObject* args);

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
extern PyObject* CherrySession_new(PyTypeObject* type, PyObject* args, PyObject* kwargs);
extern int CherrySession_init(CherrySessionObject* self, PyObject* args, PyObject* kwargs);
extern void CherrySession_dealloc(CherrySessionObject* self);

extern PyObject* CherrySession_start(CherrySessionObject* self, PyObject* args);
extern PyObject* CherrySession_stop(CherrySessionObject* self, PyObject* args);
extern PyObject* CherrySession_is_active(CherrySessionObject* self, PyObject* args);
extern PyObject* CherrySession_send(CherrySessionObject* self, PyObject* args);
extern PyObject* CherrySession_receive(CherrySessionObject* self, PyObject* args, PyObject* kwargs);
extern PyObject* CherrySession_set_timeout(CherrySessionObject* self, PyObject* args);
extern PyObject* CherrySession_get_timeout(CherrySessionObject* self, PyObject* args);

// LogLevel Enum
typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_CRITICAL
} CherryLogLevel;

// Logging functions
extern PyObject* cherry_set_log_level(PyObject* self, PyObject* args);
extern PyObject* cherry_get_log_level(PyObject* self, PyObject* args);

void register_module();

#endif // CHERRY_H
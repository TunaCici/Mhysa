#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdlib.h>

extern PyObject* go_add(PyObject*, PyObject**, Py_ssize_t, PyObject*);

static PyMethodDef MvrMethods[] = {
	{
		"add",
		(PyCFunction)(void (*)(void)) go_add,
		METH_FASTCALL | METH_KEYWORDS,
		PyDoc_STR("add(a, b) -> a + b")
	},
	{
		NULL,
		NULL,
		0,
		NULL
	}
};

static struct PyModuleDef MvrModule = {
	PyModuleDef_HEAD_INIT,
	"maverick",
	"Built-in module implemented in Go",
	-1,
	MvrMethods,
};

static PyMODINIT_FUNC PyInit_maverick(void)
{
	return PyModule_Create(&MvrModule);
}

static void register_maverick(void)
{
	PyImport_AppendInittab("maverick", PyInit_maverick);
}
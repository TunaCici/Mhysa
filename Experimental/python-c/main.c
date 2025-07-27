// main.c
#include <Python.h>
#include <stdio.h>

extern PyMODINIT_FUNC PyInit_cherry(void);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <script.py>\n", argv[0]);
        return 1;
    }

    wchar_t *program = Py_DecodeLocale(argv[0], NULL);
    if (program == NULL) {
        fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
        return 1;
    }

    PyImport_AppendInittab("cherry", PyInit_cherry);

    Py_SetProgramName(program);
    Py_Initialize();

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "Could not open file %s\n", argv[1]);
        Py_Finalize();
        return 1;
    }

    int result = PyRun_SimpleFile(fp, argv[1]);
    fclose(fp);

    if (result != 0) {
        fprintf(stderr, "Failed to execute Python script.\n");
    }

    Py_Finalize();
    PyMem_RawFree(program);
    return result;
}
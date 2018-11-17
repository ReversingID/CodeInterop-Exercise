/**
Membuat ekstensi Python dengan C.

Diuji pada: 
    - Python 3.7.1

Automatic build by distutils

Compile:
    $ python setup.py build
    $ python setup.py install

*/
#include <Python.h>
#include <stdio.h>

// Function 1: fungsi 'hello world'
static PyObject* helloworld(PyObject* self, PyObject* args)
{
    printf("Hello World\n");
    return Py_None;
}

// Function 2: Fungsi sederhana yang mengembalikan value
static PyObject* formula(PyObject* self, PyObject* args)
{
    int n; 
    
    // parse argument, simpan di "n"
    if (!PyArg_ParseTuple(args, "i", &n))
        return NULL;
        
    // return super heavy computation formula
    return Py_BuildValue("i", 135);
}



// Function mapping table
static PyMethodDef iop_methods[] = {
    { "helloworld", helloworld, METH_NOARGS, "Print Hello World" },
    { "formula", formula, METH_VARARGS, "Compute heavyweight algorithm"},
    { NULL, NULL, 0, NULL }
};

// Module definition structure
static struct PyModuleDef iop_module = {
    PyModuleDef_HEAD_INIT,
    "my_module",
    "Interop Example (extension)",
    -1,
    iop_methods
};

// Initialize the module
PyMODINIT_FUNC PyInit_my_module(void)
{
    return PyModule_Create(&iop_module);
}
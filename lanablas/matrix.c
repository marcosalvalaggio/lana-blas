#include <Python.h>
#include <cblas.h>


typedef struct {
    PyObject_HEAD
    int rows;
    int cols;
    double** data;
} MatrixObject;


static PyObject* Matrix_zeros(PyTypeObject* type, PyObject* args) {
    
    int rows, cols;
    if (!PyArg_ParseTuple(args, "ii", &rows, &cols)) {
        PyErr_SetString(PyExc_TypeError, "Invalid arguments");
        return NULL;
    }

    MatrixObject* matrix = (MatrixObject*)type->tp_alloc(type, 0);
    if (matrix == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to allocate memory for matrix");
        return NULL;
    }

    matrix->rows = rows;
    matrix->cols = cols;
    matrix->data = malloc(rows * sizeof(double*));
    for (int i = 0; i < rows; i++) {
        matrix->data[i] = malloc(cols * sizeof(double));
        for (int j = 0; j < cols; j++) {
            matrix->data[i][j] = 0.0;
        }
    }

    return (PyObject*)matrix;
}


static void Matrix_dealloc(MatrixObject* self) {
    for (int i = 0; i < self->rows; i++) {
        free(self->data[i]);
    }
    free(self->data);
    Py_TYPE(self)->tp_free((PyObject*)self);
}


static PyMethodDef MatrixExtensionMethods[] = {
    {"zeros", (PyCFunction)Matrix_zeros, METH_VARARGS | METH_CLASS, "Create a matrix of zeros"},
    {NULL, NULL, 0, NULL}
};


static PyTypeObject MatrixType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "Matrix",
    .tp_doc = "Matrix object",
    .tp_basicsize = sizeof(MatrixObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_dealloc = (destructor)Matrix_dealloc,
    .tp_methods = MatrixExtensionMethods,
};


static struct PyModuleDef matrix_extension_module = {
    PyModuleDef_HEAD_INIT,
    .m_name = "lanablas.matrix",
    .m_doc = "Extension module for matrix operations using BLAS",
    .m_size = -1,
};


PyMODINIT_FUNC PyInit_matrix(void) {
    PyObject* m;
    if (PyType_Ready(&MatrixType) < 0)
        return NULL;

    m = PyModule_Create(&matrix_extension_module);
    if (m == NULL)
        return NULL;

    Py_INCREF(&MatrixType);
    PyModule_AddObject(m, "Matrix", (PyObject*)&MatrixType);
    return m;
}

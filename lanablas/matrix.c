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


static PyObject* Matrix_ones(PyTypeObject* type, PyObject* args) {
    
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
            matrix->data[i][j] = 1.0;
        }
    }

    return (PyObject*)matrix;
}


static PyObject* Matrix_eye(PyTypeObject* type, PyObject* args) {
    
    int rows;
    if (!PyArg_ParseTuple(args, "i", &rows)) {
        PyErr_SetString(PyExc_TypeError, "Invalid arguments");
        return NULL;
    }

    MatrixObject* matrix = (MatrixObject*)type->tp_alloc(type, 0);
    if (matrix == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to allocate memory for matrix");
        return NULL;
    }

    matrix->rows = rows;
    matrix->cols = rows;
    matrix->data = malloc(rows * sizeof(double*));
    for (int i = 0; i < rows; i++) {
        matrix->data[i] = malloc(rows * sizeof(double));
        for (int j = 0; j < rows; j++) {
            if (i == j) {
                matrix->data[i][j] = 1.0;
            } else {
                matrix->data[i][j] = 0.0;
            }
        }
    }

    return (PyObject*)matrix;
}

static PyObject* Matrix_shape(MatrixObject* self) {
    PyObject* shape = PyTuple_New(2);
    PyObject* rows = PyLong_FromLong(self->rows);
    PyObject* cols = PyLong_FromLong(self->cols);
    PyTuple_SetItem(shape, 0, rows);
    PyTuple_SetItem(shape, 1, cols);
    return shape;
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
    {"ones", (PyCFunction)Matrix_ones, METH_VARARGS | METH_CLASS, "Create a matrix of ones"},
    {"eye", (PyCFunction)Matrix_eye, METH_VARARGS | METH_CLASS, "Create an identity matrix"},
    {NULL, NULL, 0, NULL}
};


static PyGetSetDef Matrix_getsetters[] = {
    {"shape", (getter)Matrix_shape, NULL, "Return the shape of the matrix as a tuple", NULL},
    {NULL}
};


static PyObject* Matrix_repr(MatrixObject* self) {
    PyObject* result = PyUnicode_New(0, 0);
    for (int i = 0; i < self->rows; i++) {
        if (i == 0) {
            PyUnicode_AppendAndDel(&result, PyUnicode_FromString("Matrix(["));
        }
        PyUnicode_AppendAndDel(&result, PyUnicode_FromString("["));
        for (int j = 0; j < self->cols; j++) {
            if (j != 0) {
                PyUnicode_AppendAndDel(&result, PyUnicode_FromString(", "));
            }
            PyObject* value = PyFloat_FromDouble(self->data[i][j]);
            PyUnicode_AppendAndDel(&result, PyObject_Str(value));
            Py_DECREF(value);
        }
        PyUnicode_AppendAndDel(&result, PyUnicode_FromString("]"));
        if (i != self->rows - 1) {
            PyUnicode_AppendAndDel(&result, PyUnicode_FromString(",\n       "));
        } else {
            PyUnicode_AppendAndDel(&result, PyUnicode_FromString("])"));
        }
    }
    return result;
}



static PyTypeObject MatrixType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "Matrix",
    .tp_doc = "Matrix object",
    .tp_basicsize = sizeof(MatrixObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_dealloc = (destructor)Matrix_dealloc,
    .tp_repr = (reprfunc)Matrix_repr,
    .tp_methods = MatrixExtensionMethods,
    .tp_getset = Matrix_getsetters,
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

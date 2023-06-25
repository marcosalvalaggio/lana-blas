//TODO - Matrix_pow
//TODO - Matrix_matmul
//TODO - Matrix_truediv
#include <Python.h>
#include <cblas.h>


typedef struct {
    PyObject_HEAD
    int rows;
    int cols;
    double** data;
} MatrixObject;

static PyTypeObject MatrixType;

static PyObject* Matrix_new(PyTypeObject* type, PyObject* args) {

    PyObject* pyList;
    if (!PyArg_ParseTuple(args, "O", &pyList)) {
        PyErr_SetString(PyExc_TypeError, "Invalid argument: expected a list");
        return NULL;
    }

    if (!PyList_Check(pyList)) {
        PyErr_SetString(PyExc_TypeError, "Invalid argument: expected a list");
        return NULL;
    }

    int rows = PyList_Size(pyList);
    int cols = PyList_Size(PyList_GetItem(pyList, 0));

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
        PyObject* row = PyList_GetItem(pyList, i);

        for (int j = 0; j < cols; j++) {
            PyObject* value = PyList_GetItem(row, j);
            matrix->data[i][j] = PyFloat_AsDouble(value);
        }
    }

    return (PyObject*)matrix;
}


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


static PyObject* Matrix_fill(PyTypeObject* type, PyObject* args) {
    
    PyObject* sizeTuple;
    double fill_value;

    if (!PyArg_ParseTuple(args, "Od", &sizeTuple, &fill_value)) {
        PyErr_SetString(PyExc_TypeError, "Invalid arguments: (rows, cols), fill_value");
        return NULL;
    }

    int rows, cols;
    if (!PyArg_ParseTuple(sizeTuple, "ii", &rows, &cols)) {
        PyErr_SetString(PyExc_TypeError, "Invalid size tuple: (rows, cols)");
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
            matrix->data[i][j] = fill_value;
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


static PyObject* Matrix_to_list(MatrixObject* self) {
    PyObject* result = PyList_New(self->rows);
    for (int i = 0; i < self->rows; i++) {
        PyObject* row = PyList_New(self->cols);
        for (int j = 0; j < self->cols; j++) {
            PyObject* value = PyFloat_FromDouble(self->data[i][j]);
            PyList_SetItem(row, j, value);
        }
        PyList_SetItem(result, i, row);
    }
    return result;
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
    {"new", (PyCFunction)Matrix_new, METH_VARARGS | METH_CLASS, "Create a matrix from a Python list"},
    {"fill", (PyCFunction)Matrix_fill, METH_VARARGS | METH_CLASS, "Fill the matrix with a specified value"},
    {"to_list", (PyCFunction)Matrix_to_list, METH_NOARGS, "Convert the matrix to a list of lists"},
    {NULL, NULL, 0, NULL}
};


static PyGetSetDef Matrix_getsetters[] = {
    {"shape", (getter)Matrix_shape, NULL, "Return the shape of the matrix as a tuple", NULL},
    {NULL}
};


PyObject * Matrix_add(PyObject *self, PyObject *other) {
    // Cast the input objects to MatrixObject
    MatrixObject* selfMatrix = (MatrixObject*)self;
    MatrixObject* otherMatrix = (MatrixObject*)other;

    // Check if the dimensions of the matrices are compatible for addition
    if (selfMatrix->rows != otherMatrix->rows || selfMatrix->cols != otherMatrix->cols) {
        PyErr_SetString(PyExc_ValueError, "Matrix dimensions are not compatible for addition");
        return NULL;
    }

    // Create a new MatrixObject for the result
    MatrixObject* result = (MatrixObject*)MatrixType.tp_alloc(&MatrixType, 0);
    if (result == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for result matrix");
        return NULL;
    }

    result->rows = selfMatrix->rows;
    result->cols = selfMatrix->cols;
    result->data = malloc(result->rows * sizeof(double*));
    for (int i = 0; i < result->rows; i++) {
        result->data[i] = malloc(result->cols * sizeof(double));
        for (int j = 0; j < result->cols; j++) {
            result->data[i][j] = selfMatrix->data[i][j] + otherMatrix->data[i][j];
        }
    }

    return (PyObject*)result;

}


PyObject * Matrix_subtract(PyObject *self, PyObject *other) {
    // Cast the input objects to MatrixObject
    MatrixObject* selfMatrix = (MatrixObject*)self;
    MatrixObject* otherMatrix = (MatrixObject*)other;

    // Check if the dimensions of the matrices are compatible for addition
    if (selfMatrix->rows != otherMatrix->rows || selfMatrix->cols != otherMatrix->cols) {
        PyErr_SetString(PyExc_ValueError, "Matrix dimensions are not compatible for addition");
        return NULL;
    }

    // Create a new MatrixObject for the result
    MatrixObject* result = (MatrixObject*)MatrixType.tp_alloc(&MatrixType, 0);
    if (result == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for result matrix");
        return NULL;
    }

    result->rows = selfMatrix->rows;
    result->cols = selfMatrix->cols;
    result->data = malloc(result->rows * sizeof(double*));
    for (int i = 0; i < result->rows; i++) {
        result->data[i] = malloc(result->cols * sizeof(double));
        for (int j = 0; j < result->cols; j++) {
            result->data[i][j] = selfMatrix->data[i][j] - otherMatrix->data[i][j];
        }
    }

    return (PyObject*)result;

}


PyObject * Matrix_mul(PyObject *self, PyObject *other) {
    // Cast the input objects to MatrixObject
    MatrixObject* selfMatrix = (MatrixObject*)self;
    MatrixObject* otherMatrix = (MatrixObject*)other;

    // Check if the dimensions of the matrices are compatible for addition
    if (selfMatrix->rows != otherMatrix->rows || selfMatrix->cols != otherMatrix->cols) {
        PyErr_SetString(PyExc_ValueError, "Matrix dimensions are not compatible for addition");
        return NULL;
    }

    // Create a new MatrixObject for the result
    MatrixObject* result = (MatrixObject*)MatrixType.tp_alloc(&MatrixType, 0);
    if (result == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for result matrix");
        return NULL;
    }

    result->rows = selfMatrix->rows;
    result->cols = selfMatrix->cols;
    result->data = malloc(result->rows * sizeof(double*));
    for (int i = 0; i < result->rows; i++) {
        result->data[i] = malloc(result->cols * sizeof(double));
        for (int j = 0; j < result->cols; j++) {
            result->data[i][j] = selfMatrix->data[i][j] * otherMatrix->data[i][j];
        }
    }

    return (PyObject*)result;

}


PyObject * Matrix_negative(PyObject *self) {
    // Cast the input objects to MatrixObject
    MatrixObject* selfMatrix = (MatrixObject*)self;

    // Create a new MatrixObject for the result
    MatrixObject* result = (MatrixObject*)MatrixType.tp_alloc(&MatrixType, 0);
    if (result == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for result matrix");
        return NULL;
    }

    result->rows = selfMatrix->rows;
    result->cols = selfMatrix->cols;
    result->data = malloc(result->rows * sizeof(double*));
    for (int i = 0; i < result->rows; i++) {
        result->data[i] = malloc(result->cols * sizeof(double));
        for (int j = 0; j < result->cols; j++) {
            result->data[i][j] = selfMatrix->data[i][j] * -1;
        }
    }

    return (PyObject*)result;

}


PyNumberMethods Matrix_as_number = {
    Matrix_add,
    Matrix_subtract,
    Matrix_mul,
    0,
    0,
    0,
    Matrix_negative,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
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
    .tp_as_number = &Matrix_as_number,
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

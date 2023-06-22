#include <Python.h>
#include <cblas.h>


typedef struct {
    PyObject_HEAD
    int rows;
    int cols;
    double* data;
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
    matrix->data = malloc(rows * sizeof(double *));
    for (int i=0; i<rows; i++) {
        matrix->data[i] = malloc(cols * sizeof(double));
    }
    // access the matrix and populate
    for (int i=0; i<rows; i++) {
        for (int j=0; j<cols; j++){
        matrix->data[i][j] = 0.0;
        }
    }


    return (PyObject*)matrix;

}

static PyObject* multiply_matrices(PyObject* self, PyObject* args) {
    // Parse the arguments
    int n;
    if (!PyArg_ParseTuple(args, "i", &n)) {
        PyErr_SetString(PyExc_TypeError, "Invalid arguments");
        return NULL;
    }

    // Allocate memory for matrices A, B, and C
    double* A = (double*)malloc(n * n * sizeof(double));
    double* B = (double*)malloc(n * n * sizeof(double));
    double* C = (double*)malloc(n * n * sizeof(double));

    // Initialize matrices A and B as diagonal matrices
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A[i * n + j] = (i == j) ? 1.0 : 0.0;
            B[i * n + j] = (i == j) ? 2.0 : 0.0;
        }
    }

    // Perform matrix multiplication using BLAS
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0, A, n, B, n, 0.0, C, n);

    // Create a Python list to store the resultant matrix C
    PyObject* result = PyList_New(n);
    
    for (int i = 0; i < n; i++) {
        PyObject* row = PyList_New(n);
        for (int j = 0; j < n; j++) {
            PyObject* value = PyFloat_FromDouble(C[i * n + j]);
            PyList_SET_ITEM(row, j, value);
        }
        PyList_SET_ITEM(result, i, row);
    }

    // Free allocated memory
    free(A);
    free(B);
    free(C);

    // Return the resultant matrix
    return result;
}


static PyMethodDef MatrixExtensionMethods[] = {
    {"zeros", (PyCFunction)Matrix_zeros, METH_VARARGS | METH_CLASS, "Create a matrix of zeros"},
    //{"multiply_matrices", multiply_matrices, METH_VARARGS, "Multiply two matrices using BLAS"},
    {NULL, NULL, 0, NULL}
};


static void Matrix_dealloc(MatrixObject* self) {
    free(self->data);
    Py_TYPE(self)->tp_free((PyObject*)self);
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

#include <Python.h>
#include <cblas.h>

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
    {"multiply_matrices", multiply_matrices, METH_VARARGS, "Multiply two matrices using BLAS"},
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef matrix_extension_module = {
    PyModuleDef_HEAD_INIT,
    "lanablas.matrix",
    "Extension module for matrix multiplication using BLAS",
    -1,
    MatrixExtensionMethods,
    NULL,
    NULL,
    NULL,
    NULL
};

PyMODINIT_FUNC PyInit_matrix(void) {
    
    PyObject *m;
    m = PyModule_Create(&matrix_extension_module);
    if (m == NULL) {
        return NULL;
    }

    return m;
}

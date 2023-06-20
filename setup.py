from distutils.core import setup, Extension

module_extension = Extension(
    'matrix_extension',
    sources=['matrix.c'],
    libraries=['openblas'],  # Link against the BLAS library
    extra_compile_args=['-std=c11'],  # Set the C standard to C11
    include_dirs=['/usr/local/opt/openblas/include'],  # BLAS include directory
    library_dirs=['/usr/local/opt/openblas/lib'],  # BLAS library directory
)

setup(
    name='matrix_extension',
    version='1.0',
    description='Extension module for matrix multiplication',
    ext_modules=[module_extension]
)

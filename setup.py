import os
from distutils.core import setup, Extension


directory = os.path.abspath(os.path.dirname(__file__))
with open(os.path.join(directory, 'README.md'), encoding='utf-8') as f:
    long_description = f.read()


module_extension = Extension(
    'matrix',
    sources=['lanablas/matrix.c'],
    libraries=['openblas'],  # Link against the BLAS library
    extra_compile_args=['-std=c11'],  # Set the C standard to C11
    include_dirs=['/usr/local/opt/openblas/include'],  # BLAS include directory
    library_dirs=['/usr/local/opt/openblas/lib'],  # BLAS library directory
)

setup(
name='lanablas',
    version='0.0.2',
    description='Extension module for matrix multiplication',
    author='Marco Salvalaggio',
    author_email='mar.salvalaggio@gmail.com',
    license='MIT',
    long_description=long_description,
    long_description_content_type='text/markdown',
    packages=['lanablas'],
    ext_modules=[module_extension],
    classifiers=[
          "Programming Language :: Python :: 3",
          "License :: OSI Approved :: MIT License",
          "Operating System :: OS Independent",
      ]
)

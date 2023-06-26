import os
from setuptools.command.build_ext import build_ext
from setuptools import setup, Extension

directory = os.path.abspath(os.path.dirname(__file__))
with open(os.path.join(directory, 'README.md'), encoding='utf-8') as f:
    long_description = f.read()

class CustomBuildExt(build_ext):
    def run(self):
        self.include_dirs.append(os.path.join(os.getcwd(), 'openblas', 'include'))
        self.library_dirs.append(os.path.join(os.getcwd(), 'openblas', 'lib'))
        super().run()


module_extension = Extension(
    'matrix',
    sources=['lanablas/matrix.c'],
    libraries=['openblas'],  # Link against the BLAS library
    extra_compile_args=['-std=c11'],
    include_dirs=['openblas/include'],  # BLAS include directory within your package
)

setup(
    name='lanablas',
    version='0.1.0',
    description='Extension module for matrix multiplication',
    author='Marco Salvalaggio',
    author_email='mar.salvalaggio@gmail.com',
    license='MIT',
    long_description=long_description,
    long_description_content_type='text/markdown',
    packages=['lanablas'],
    ext_modules=[module_extension],
    cmdclass={'build_ext': CustomBuildExt},  # Use the custom build_ext command
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
    include_package_data=True,  # Include package data specified in MANIFEST.in
)
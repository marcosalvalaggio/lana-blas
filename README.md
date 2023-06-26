## LanaBLAS 🐑

[![stability-wip](https://img.shields.io/badge/stability-wip-lightgrey.svg)](https://github.com/mkenney/software-guides/blob/master/STABILITY-BADGES.md#work-in-progress) [![macOS](https://img.shields.io/badge/macos-compatible-brightgreen.svg)](https://github.com/your-repo) [![Windows](https://img.shields.io/badge/windows-not%20compatible-red.svg)](https://github.com/your-repo) [![Linux](https://img.shields.io/badge/linux-not%20compatible-red.svg)](https://github.com/your-repo) [![Tests Passing](https://img.shields.io/badge/tests-passing-brightgreen.svg)](https://github.com/your-repo) [![PyPI - Status](https://img.shields.io/pypi/v/lanablas.svg)](https://pypi.org/project/lanablas/) [![Project Status](https://img.shields.io/badge/status-broken-red)](https://github.com/USER/REPO)




**L***inear* **A**lgebra for **n***octurnal* *and* **a**dventurous *data scientists exploring **BLAS***


**LanaBLAS** is a repository primarily used for testing the development of CPython extension modules with external dependencies, such as ([OpenBLAS](https://github.com/xianyi/OpenBLAS)). It's a work-in-progress memetic project aimed at having fun and learning new things.


## Install 

Actually, the package is currently only available for macOS x86. Wheels for ARM and Linux platforms will be added to the PyPI registry as soon as possible.

```console
pip install lanablas==0.1.0
```

## Example 

```python
from lanablas import Matrix

a = Matrix.ones(3,3)
print(a)
print(type(a), a.shape)

b = Matrix.eye(3)
print(b)
print(type(b), b.shape)

c = a + b
print(c)
print(type(c), b.shape)
```

For more comprehensive examples, please visit the [examples](https://github.com/marcosalvalaggio/lana-blas/tree/main/examples) folder


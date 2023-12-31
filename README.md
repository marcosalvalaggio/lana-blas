## LanaBLAS 🐑

![maintenance-status](https://img.shields.io/badge/maintenance-experimental-blue.svg) [![Tests Passing](https://img.shields.io/badge/tests-passing-brightgreen.svg)](https://github.com/your-repo) [![macOS](https://img.shields.io/badge/Platform-macOS-9cf)](https://www.apple.com/macos/)



**L***inear* **A**lgebra for **n***octurnal* *and* **a**dventurous *data scientists exploring **BLAS***

**LanaBLAS** is a repository primarily used for testing the development of CPython extension modules with external dependencies, such as ([OpenBLAS](https://github.com/xianyi/OpenBLAS)). It's a work-in-progress memetic project aimed at having fun and learning new things.


## Install

```console
brew update
brew install openblas
export BLA_VENDOR=OpenBLAS
pip install lanablas==0.1.7
```

## Example 

```python
from lanablas import Matrix, inject

a = Matrix.ones(3,3)
print(a)
print(type(a), a.shape)

b = Matrix.eye(3)
print(b)
print(type(b), b.shape)

c = a + b
print(c)
print(type(c), b.shape)

for row in c.tolist():
    print(row, type(row))

# sub-matrix
d = Matrix.new(inject(c.tolist()[0]))
print(type(d), d.shape)
```

For more comprehensive examples, please visit the [examples](https://github.com/marcosalvalaggio/lana-blas/tree/main/examples) folder.




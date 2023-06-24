from lanablas import Matrix, inject

a = Matrix.eye(3)
b = Matrix.eye(3)
try:
    c = a + b
    print(c)
except Exception as e:
    print(e)
try:
    c = a.__add__(b)
    print(c)
except Exception as e:
    print(e)

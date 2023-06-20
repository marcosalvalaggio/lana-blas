import time 
import lanablas as lb
import numpy as np

# lab
s = time.monotonic()
result = lb.multiply_matrices(2000)
e = time.monotonic()
t = e - s
print(f"lanablas matmul: {t:.2f} sec.")

# numpy 
a = np.eye(2000)
b = np.eye(2000)
s = time.monotonic()
result = a @ b
e = time.monotonic()
t = e - s
print(f"numpy matmul: {t:.2f} sec.")
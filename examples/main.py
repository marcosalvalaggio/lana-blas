import time 
import lanablas as lb
import numpy as np

# lab
s = time.monotonic()
result = lb.multiply_matrices(4096)
e = time.monotonic()
t = e - s
print(f"lanablas matmul: {t:.2f} sec.")

# numpy 
a = np.eye(4096)
b = np.eye(4096)
s = time.monotonic()
result = a @ b
e = time.monotonic()
t = e - s
print(f"numpy matmul: {t:.2f} sec.")
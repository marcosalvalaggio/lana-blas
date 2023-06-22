from lanablas import Matrix

zeros = Matrix.zeros(3,3)
print(zeros)
print(type(zeros), zeros.shape)

zeros = Matrix.ones(3,3)
print(zeros)
print(type(zeros), zeros.shape)

eye = Matrix.eye(3)
print(eye)
print(type(eye), eye.shape)
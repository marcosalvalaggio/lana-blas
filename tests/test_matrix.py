from lanablas import Matrix, inject
import numpy as np
import unittest

class TestValue(unittest.TestCase):

    def test_ops(self):
        # lanablas
        a = Matrix.eye(3)
        b = Matrix.eye(3)
        c = a + b
        d = c - a
        e = d * d
        f = -e
        # numpy 
        anp = np.eye(3)
        bnp = np.eye(3)
        cnp = anp + bnp
        dnp = cnp - anp
        enp = dnp * dnp
        fnp = -enp
        # test
        res = f.tolist()
        res_np = fnp.tolist()
        self.assertEqual(res, res_np)

    def test_ops_2(self):
        # lanablas 
        a = Matrix.full((3,3),3)
        b = a ** 2
        c = b / a
        res = c.tolist()
        # numpy 
        a = np.full((3,3),3)
        b = a ** 2
        c = b / a
        res_np = c.tolist()
        # test 
        self.assertEqual(res, res_np)


    def test_matmul(self):
        # lanablas
        a = Matrix.full((2,3),1)
        b = Matrix.full((3,5),2)
        c = a.__matmul__(b)
        res = c.tolist()
        # numpy 
        a = np.full((2,3),fill_value=1.0)
        b = np.full((3,5),fill_value=2.0)
        c = a @ b
        res_np = c.tolist()
        # test
        self.assertEqual(res, res_np)



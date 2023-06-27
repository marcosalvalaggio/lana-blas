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
        res = f.to_list()
        res_np = fnp.tolist()
        self.assertEqual(res, res_np)

    def test_ops_2(self):
        # lanablas 
        a = Matrix.fill((3,3),3)
        b = a ** 2
        c = b / a
        res = c.to_list()
        # numpy 
        a = np.full((3,3),3)
        b = a ** 2
        c = b / a
        res_np = c.tolist()
        # test 
        self.assertEqual(res, res_np)



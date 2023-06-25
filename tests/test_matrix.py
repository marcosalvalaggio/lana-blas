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



#
# Copyright 2014-2018 Neueda Ltd.
#
from cdr import Cdr
import unittest
from datetime import datetime


field1 = 1
field2 = 2
field3 = 55


class TestCdr(unittest.TestCase):

    def get_a_cdr(self):
        d = Cdr()
        d.setInteger(field1, 123)
        d.setString(field2, "Hello")
        d.setString(field3, "World")

        return d

    def test_set_integer(self):
        d = self.get_a_cdr()
        self.assertEqual(d.getInt32(field1), 123)

    def test_set_string(self):
        d = self.get_a_cdr()
        d.setString(field2, "Hello")
        self.assertEqual(d.getString(field2), "Hello")

    def test_get_exception(self):
        d = self.get_a_cdr()
        with self.assertRaises(RuntimeError):
            d.getInteger(4)

    def test_to_string(self):
        d = Cdr()
        d.setInteger(field1, 123)
        self.assertEqual(d.toString(), "1=123")

    def test_str(self):
        d = Cdr()
        d.setInteger(field1, 123)

    def test_nested(self):
        d = Cdr()
        e = Cdr()
        e.setString(1, "hello")
        e.setString(2, "world")
        d.appendArray(1, e)

        f = d.getArray(1)
        self.assertEqual(e.getString(1), f[0].getString(1))
        self.assertEqual(e.getString(2), f[0].getString(2))

    def test_datetime(self):
        x = datetime.now()
        d = Cdr()
        d[52] = x

        self.assertEqual(d[52], x)

    def test_cdrarray(self):
        d = Cdr()
        d[52] = "AAPL"

        e = Cdr()
        e[52] = "FB"

        f = Cdr()
        f[0] = [d, e]

        r = Cdr()
        r[1000] = f

    def test_to_python_dict(self):
        d = Cdr()
        e = Cdr()
        f = Cdr()

        f[21] = 400

        e[11] = 300
        e[12] = [f]

        d[1] = 100
        d[2] = 200

        d[3] = [e]
        assert(d.toPythonDict()[3][0][12][0][21] == 400)


if __name__ == '__main__':
    unittest.main()

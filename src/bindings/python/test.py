"""
    Copyright 2014-2018 Neueda Ltd.

    Generated 17/04/18
"""
from CommonDataRepresentation import cdr
import unittest


field1 = 1
field2 = 2
field3 = 55


class TestCdr(unittest.TestCase):

    def get_a_cdr(self):
        d = cdr()
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
        d = cdr()
        d.setInteger(field1, 123)
        self.assertEqual(d.toString(), "1=123")

    def test_str(self):
        d = cdr()
        d.setInteger(field1, 123)


if __name__ == '__main__':
    unittest.main()

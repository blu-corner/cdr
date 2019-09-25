#
# Copyright 2014-2018 Neueda Ltd.
#
from cdr import Cdr
import unittest
from datetime import datetime


class TestSerializeCdr(unittest.TestCase):

    def get_a_cdr(self, now):
        d = Cdr()

        e = Cdr()
        e[11] = 123
        e[12] = "hi"

        f = Cdr()
        f[21] = 456
        f[22] = "yo"

        d[1] = 123
        d[2] = "Hello"
        d[3] = "World"
        d[4] = 1.25
        d[5] = now

        d[6] = [e, f]

        return d

    def test_serialize(self):
        d = self.get_a_cdr(datetime.now())

        data = d.serialize()

        e = Cdr()
        e.deserialize(data)

        self.assertEqual(e.keys(), d.keys())


if __name__ == '__main__':
    unittest.main()

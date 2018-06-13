#
# Copyright 2014-2018 Neueda Ltd.
#
# An example showing the functionality within the python programming language.
# Note the class has been renamed in order to be inline with Python standards.
from cdr import Cdr

d = Cdr()
d.setInteger(1, 100)  # set an integer value
d.setString(2, "hello")  # set a string value

# retrieve an integer
val = d.getInteger(1)
print("get integer: {0} ({1})".format(val, type(val)))

# retrieve an integer as string
val = d.getString(1)
print("get integer as string: {0} ({1})".format(val, type(val)))

# retrieve a string
val = d.getString(2)
print("get string: {0}".format(val))

# try to retrieve an integer from a key with value that cannot be converted to
# integer
try:
    val = d.getInteger(2)
except Exception as e:
    print('retrieve integer from a non-numeric string: {0}'.format(e))

# attempt to retrieve a value for a non-existant key
try:
    val = d.getString(0)
except Exception as e:
    print('retrieve non existant key: {0}'.format(e))

# nested cdr example
e = Cdr()
e.setString(1, "nested")
e.setString(2, "cdr")
d.appendArray(3, e)

f = d.getArray(3)
print("nested-cdr: {0}".format(f[0].toString()))

# output the cdr to a readable string
print("cdr.toString(): {0}".format(d.toString()))

from Cdr import cdr

field1 = 1
field2 = 2
field3 = 55

d = cdr()
d.setInteger(field1, 123)

print d.toString()

d.setString(field2, "Hello")
d.setString(field3, "World")

print str(d)

print d.getInt32(1)

print d.getString(field3)

# this should be a string
try:
    print d.getInt32(3)
except Exception as e:
    print e

print "done"
